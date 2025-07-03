/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:22:54 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 10:26:30 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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

t_client_state	*get_client_instance(void)
{
	static t_client_state	instance;
	static int				initialized = 0;

	if (!initialized)
	{
		ft_memset(&instance, 0, sizeof(t_client_state));
		instance.getting_header = 1;
		instance.msg.size_message = 0;
		instance.transmission_active = 0;
		instance.queue_position = 0;
		instance.sequence_number = 0;
		instance.expected_checksum = 0;
		instance.calculated_checksum = 0;
		initialized = 1;
	}
	return (&instance);
}

void	reset_client_state(t_client_state *client)
{
	pid_t	old_pid = client->actual_pid;
	int		was_active = client->transmission_active;
	
	if (client->msg.message)
	{
		free(client->msg.message);
		client->msg.message = NULL;
	}
	ft_bzero(client, sizeof(t_client_state));
	client->getting_header = 1;
	client->msg.size_message = 0;
	client->transmission_active = 0;
	
	if (was_active && old_pid > 0)
		log_msg(LOG_INFO, "Released transmission slot from client PID %d", old_pid);
}

int	is_server_busy(void)
{
	t_client_state	*client;

	client = get_client_instance();
	return (client->transmission_active && client->actual_pid > 0);
}

void	set_server_busy(pid_t client_pid)
{
	t_client_state	*client;

	client = get_client_instance();
	client->transmission_active = 1;
	client->actual_pid = client_pid;
	log_msg(LOG_INFO, "Server now busy with client PID %d", client_pid);
}

int	get_bit_value(int signum)
{
	if (signum == SIGUSR1)
		return (0);
	return (1);
}

void	memory_reserve_to_store_signals(void)
{
	t_client_state	*client;

	client = get_client_instance();
	ft_printf("SIZE_MSG: [%d]\n", client->msg.size_message);
	log_msg(LOG_INFO, "Allocating memory for message of %d bytes",
		client->msg.size_message);
	client->msg.message = malloc((client->msg.size_message + 1) * 1);
	if (client->msg.message == NULL)
	{
		log_msg(LOG_ERROR, "Memory allocation failed for %d bytes",
			client->msg.size_message);
		write(2, "Error: Malloc failed\n", 21);
		exit(EXIT_FAILURE);
	}
	client->getting_header = 0;
	client->getting_msg = 1;
	client->sig_count = 0;
	log_msg(LOG_SUCCESS, "Memory allocated, "
		"switching to message reception mode");
}

void	handle_header(int signum)
{
	const int		bit_value = get_bit_value(signum);
	t_client_state	*client;

	client = get_client_instance();
	
	if (client->sig_count == 0)
	{
		client->msg.size_message = 0;
		log_msg(LOG_DEBUG, "Starting header reception (message length)");
	}
	
	if (client->sig_count < HEADER_SIZE)
	{
		// Fix the bit position calculation
		int bit_position = HEADER_SIZE - 1 - client->sig_count;
		client->msg.size_message |= (bit_value << bit_position);
		client->sig_count++;
		
		log_msg(LOG_DEBUG, "Header bit %d/%d: %d (bit_pos: %d, current size: %d)",
			client->sig_count, HEADER_SIZE, bit_value, bit_position, client->msg.size_message);
		
		// Debug: show the bit position calculation for first few bits
		if (client->sig_count <= 5) // Show first 5 bits for debugging
		{
			log_msg(LOG_DEBUG, "Bit position: %d, value: %d, shift result: %d", 
				bit_position, bit_value, (bit_value << bit_position));
		}
	}
	
	if (client->sig_count == HEADER_SIZE)
	{
		log_msg(LOG_INFO, "Header complete: message size = %d bytes", client->msg.size_message);
		
		// Validate message size
		if (client->msg.size_message <= 0 || client->msg.size_message > 10000000)
		{
			log_msg(LOG_ERROR, "Invalid message size: %d bytes", client->msg.size_message);
			ft_printf("Error: Invalid message size received: %d\n", client->msg.size_message);
			
			// Don't clean global, just reset and continue
			reset_client_state(client);
			return ;
		}
		
		memory_reserve_to_store_signals();
	}
}
