/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:11:03 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 02:15:13 by codespace        ###   ########.fr       */
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
	if (kill(pid, signal) == -1)
	{
		ft_printf("Error: Signal sending failed\n");
		log_msg(LOG_ERROR, "Failed to send signal %d to PID %d", signal, pid);
		exit(EXIT_FAILURE);
	}
	log_msg(LOG_DEBUG, "Sent signal %s to server PID %d", signal_name, pid);
}

void	wait_for_server_ack(void)
{
	t_server_state	*server;
	int				timeout_count;

	server = get_server_instance();
	timeout_count = 0;
	log_msg(LOG_DEBUG, "Waiting for server acknowledgment...");
	while (!server->ready_to_proceed)
	{
		usleep(100);
		timeout_count++;
		if (timeout_count > 100000)
		{
			ft_printf("Error: Server acknowledgment timeout\n");
			log_msg(LOG_ERROR, "Timeout waiting for server acknowledgment"
				" after %d attempts", timeout_count);
			exit(EXIT_FAILURE);
		}
	}
	server->ready_to_proceed = 0;
	log_msg(LOG_DEBUG, "Server acknowledgment received after %d microseconds",
		timeout_count * 100);
}

void	send_bit(unsigned long long value, int i, t_client *info)
{
	int	bit_value;

	bit_value = 0;
	if (value & (1ULL << i))
		bit_value = 1;
	log_msg(LOG_DEBUG, "Sending bit %d: %d", i, bit_value);
	if (bit_value)
		send_signal(info->server_pid, CHAR_1);
	else
		send_signal(info->server_pid, CHAR_0);
	wait_for_server_ack();
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
	log_msg(LOG_INFO, "Sending %zu-bit value: %llu (0x%llx)",
		bit_length, value, value);
	i = bit_length - 1;
	while (i >= 0)
	{
		send_bit(value, i, info);
		i--;
	}
	log_msg(LOG_SUCCESS, "Successfully sent %zu bits", bit_length);
}
