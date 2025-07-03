/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:22:54 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:34:09 by codespace        ###   ########.fr       */
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
		checksum ^= data[i];
		checksum = (checksum << 1) | (checksum >> 31);
		i++;
	}
	return (checksum);
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

// Calculate bit position: MSB first (31, 30, 29, ..., 0)
void	handle_header(int signum)
{
	const int		bit_value = get_bit_value(signum);
	t_client_state	*client;
	int				bit_position;

	client = get_client_instance();
	if (client->sig_count == 0)
	{
		client->msg.size_message = 0;
		log_msg(LOG_DEBUG, "Starting header reception (message length)");
	}
	if (client->sig_count < HEADER_SIZE)
	{
		bit_position = HEADER_SIZE - 1 - client->sig_count;
		if (bit_value == 1)
			client->msg.size_message |= (1U << bit_position);
		client->sig_count++;
		log_msg(LOG_DEBUG, "Header bit %d/%d: %d (bit_pos: %d, current size: %u)",
			client->sig_count, HEADER_SIZE, bit_value, bit_position, 
			(unsigned int)client->msg.size_message);
		if (client->sig_count <= 8)
		{
			log_msg(LOG_DEBUG, "Bit position: %d, value: %d, mask: 0x%x, size so far: %u", 
				bit_position, bit_value, bit_value ? (1U << bit_position) : 0,
				(unsigned int)client->msg.size_message);
		}
	}
	if (client->sig_count == HEADER_SIZE)
	{
		log_msg(LOG_INFO, "Header complete: message size = %d bytes (0x%x)", 
			client->msg.size_message, (unsigned int)client->msg.size_message);
		if (client->msg.size_message <= 0 || client->msg.size_message > 10000000)
		{
			log_msg(LOG_ERROR, "Invalid message size: %d bytes", client->msg.size_message);
			ft_printf("Error: Invalid message size received: %d\n", client->msg.size_message);
			reset_client_state(client);
			return ;
		}
		memory_reserve_to_store_signals();
	}
}


void	keep_server_up(void)
{
	while (1)
		check_clean_status();
}
