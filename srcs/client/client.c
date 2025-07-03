/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:11:03 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 07:57:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	init_data(char **argv, t_client *data)
{
	ft_memset(data, 0, sizeof(t_client));
	data->server_pid = ft_atoi(argv[1]);
	data->client_pid = getpid();
	data->msg = argv[2];
	if (data->server_pid <= 0)
	{
		ft_printf("Error: %s\n", BAD_SIGNAL);
		log_msg(LOG_ERROR, "Invalid server PID: %s", argv[1]);
		exit(EXIT_FAILURE);
	}
	log_msg(LOG_DEBUG, "Client PID: %d, Server PID: %d", data->client_pid,
		data->server_pid);
}

void	send_signal(pid_t pid, int signal)
{
	const char	*signal_name;

	if (signal == SIGUSR1)
		signal_name = "SIGUSR1 (0)";
	else
		signal_name = "SIGUSR2 (1)";
	
	// First check if process exists
	if (kill(pid, 0) == -1)
	{
		ft_printf("Error: Server process PID %d no longer exists\n", pid);
		log_msg(LOG_ERROR, "Server process PID %d disappeared", pid);
		exit(EXIT_FAILURE);
	}
	
	if (kill(pid, signal) == -1)
	{
		ft_printf("Error: Signal sending failed\n");
		log_msg(LOG_ERROR, "Failed to send signal %d to PID %d", signal, pid);
		exit(EXIT_FAILURE);
	}
	log_msg(LOG_DEBUG, "Sent signal %s to server PID %d", signal_name, pid);
}

int	calculate_checksum(const char *data, int length)
{
	int	checksum;
	int	i;

	checksum = 0;
	i = 0;
	while (i < length)
	{
		checksum ^= data[i]; // Simple XOR checksum
		checksum = (checksum << 1) | (checksum >> 31); // Rotate left
		i++;
	}
	return (checksum);
}

void	wait_for_server_ack(void)
{
	t_server_state	*server;
	int				timeout_count;
	int				max_timeout;
	pid_t			my_pid;

	server = get_server_instance();
	my_pid = getpid();
	timeout_count = 0;
	max_timeout = 100000; // 10 seconds - increased timeout for multiple acks
	
	log_msg(LOG_DEBUG, "Waiting for server acknowledgment...");
	while (!server->ready_to_proceed)
	{
		usleep(100);
		timeout_count++;
		
		// Check transmission ownership every 100ms
		if (timeout_count % 1000 == 0)
		{
			if (!is_transmission_owner(my_pid))
			{
				log_msg(LOG_ERROR, "Lost transmission ownership while waiting for ACK");
				exit(EXIT_FAILURE);
			}
		}
		
		if (timeout_count > max_timeout)
		{
			ft_printf("Error: Server acknowledgment timeout\n");
			log_msg(LOG_ERROR, "Timeout waiting for server acknowledgment after %d attempts", 
				timeout_count);
			end_transmission();
			exit(EXIT_FAILURE);
		}
	}
	server->ready_to_proceed = 0;
	log_msg(LOG_DEBUG, "Server acknowledgment received after %d microseconds",
		timeout_count * 100);
}

void	wait_for_transmission_slot(t_client *data)
{
	t_server_state	*server;
	int				wait_count;
	pid_t			my_pid;

	(void)data; // Mark parameter as unused
	server = get_server_instance();
	my_pid = getpid();
	wait_count = 0;
	
	log_msg(LOG_INFO, "Waiting for transmission slot...");
	while (server->transmission_active && !is_transmission_owner(my_pid))
	{
		usleep(10000); // 10ms
		wait_count++;
		
		// Re-ping server every 30 seconds to check availability
		if (wait_count % 3000 == 0)
		{
			log_msg(LOG_INFO, "Still waiting for transmission slot (waited %d seconds)", 
				wait_count / 100);
			
			// Check if server process still exists
			if (kill(server->pid, 0) == -1)
			{
				ft_printf("Error: Server process PID %d no longer exists\n", server->pid);
				log_msg(LOG_ERROR, "Server process disappeared while waiting");
				exit(EXIT_FAILURE);
			}
			
			if (kill(server->pid, SIGUSR1) == -1)
			{
				log_msg(LOG_ERROR, "Server appears to be down");
				exit(EXIT_FAILURE);
			}
		}
		
		// Maximum wait time: 10 minutes
		if (wait_count > 60000)
		{
			ft_printf("Error: Transmission slot timeout\n");
			log_msg(LOG_ERROR, "Timeout waiting for transmission slot");
			exit(EXIT_FAILURE);
		}
	}
	
	// Claim the transmission slot
	set_transmission_active(my_pid);
	log_msg(LOG_SUCCESS, "Transmission slot acquired");
}

void	send_bit(unsigned long long value, int i, t_client *info)
{
	int		bit_value;
	int		retry_count;
	int		max_retries;

	bit_value = 0;
	retry_count = 0;
	max_retries = 3;
	
	if (value & (1ULL << i))
		bit_value = 1;
	
	log_msg(LOG_DEBUG, "Sending bit %d: %d", i, bit_value);
	
	// Retry loop for signal sending
	while (retry_count < max_retries)
	{
		// Send the signal
		if (bit_value)
			send_signal(info->server_pid, CHAR_1);
		else
			send_signal(info->server_pid, CHAR_0);
		
		// Wait for acknowledgment with timeout
		wait_for_server_ack();
		
		// If we got here, acknowledgment was received successfully
		return ;
	}
	
	// If we reach here, all retries failed
	ft_printf("Error: Failed to send bit after %d retries\n", max_retries);
	log_msg(LOG_ERROR, "Bit transmission failed after %d retries", max_retries);
	exit(EXIT_FAILURE);
}

void	send_signals(void *data, size_t bit_length, t_client *info)
{
	unsigned long long	value;
	int					i;

	value = 0;
	if (bit_length == 8)
		value = *((unsigned char *)data);
	else if (bit_length == 32)
		value = *((unsigned int *)data);
	
	log_msg(LOG_INFO, "Sending %zu-bit value: %llu (0x%llx)", bit_length, value, value);
	
	i = bit_length - 1;
	while (i >= 0)
	{
		send_bit(value, i, info);
		i--;
	}
	log_msg(LOG_SUCCESS, "Successfully sent %zu bits", bit_length);
}