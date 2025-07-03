/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_message.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 03:25:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 10:32:32 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	print_message(t_client_state *client)
{
	write(1, HEADER_MESSAGE, 18);
	write(1, client->msg.message, client->msg.size_message);
	write(1, "\n", 1);
}

void	handle_complete_message(t_client_state *client)
{
	pid_t	client_pid;
	int		actual_checksum;

	client->msg.message[client->msg_pos] = '\0';
	
	// Calculate checksum of received message
	actual_checksum = calculate_checksum(client->msg.message, client->msg_pos);
	
	log_msg(LOG_SUCCESS, "Message reception complete: %d characters received", client->msg_pos);
	log_msg(LOG_DEBUG, "Message checksum: calculated=%d, expected=%d", 
		actual_checksum, client->expected_checksum);
	
	// Validate checksum if it was provided
	if (client->expected_checksum != 0 && actual_checksum != client->expected_checksum)
	{
		log_msg(LOG_ERROR, "Checksum mismatch! Message may be corrupted");
		ft_printf("Warning: Message checksum failed - data may be corrupted\n");
	}
	
	print_message(client);
	client_pid = client->client_pid;
	
	// Clean up message memory
	if (client->msg.message)
	{
		free(client->msg.message);
		client->msg.message = NULL;
	}
	
	// Send multiple completion signals to ensure delivery
	if (client_pid > 0)
	{
		log_msg(LOG_INFO, "Sending completion signal to client %d", client_pid);
		// Send completion signal multiple times
		kill(client_pid, SIGUSR1);
		usleep(1000);
		kill(client_pid, SIGUSR1);
		usleep(1000);
		kill(client_pid, SIGUSR1);
	}
	
	// Reset client state and release server
	reset_client_state(client);
	log_msg(LOG_INFO, "Server released, ready for new connections");
}

void	process_character(t_client_state *client, int bit_value, int bit_pos)
{
	client->char_value |= (bit_value << (7 - bit_pos));
	client->sig_count++;
	log_msg(LOG_DEBUG, "Bit %d/8: %d (char value: %d)",
		bit_pos + 1, bit_value, client->char_value);
	
	if (client->sig_count % 8 == 0)
	{
		client->msg.message[client->msg_pos] = client->char_value;
		log_msg(LOG_DEBUG, "Character complete: '%c' (ASCII: %d) at position %d/%d",
			client->char_value, client->char_value, client->msg_pos + 1, client->msg.size_message);
		client->msg_pos++;
		client->char_value = 0; // Reset for next character
		
		if (client->msg_pos >= client->msg.size_message)
			handle_complete_message(client);
	}
}

void	handle_msg(int signum)
{
	const int		bit_value = get_bit_value(signum);
	t_client_state	*client;
	int				bit_pos;

	client = get_client_instance();
	bit_pos = client->sig_count % 8;
	
	if (bit_pos == 0)
	{
		client->char_value = 0; // Reset char_value at start of new character
		log_msg(LOG_DEBUG, "Starting new character %d/%d",
			client->msg_pos + 1, client->msg.size_message);
	}
	
	// Set the bit in the correct position (MSB first: 7, 6, 5, ..., 0)
	if (bit_value == 1)
	{
		client->char_value |= (1 << (7 - bit_pos));
	}
	
	client->sig_count++;
	log_msg(LOG_DEBUG, "Bit %d/8: %d (bit_pos: %d, char value: %d)",
		bit_pos + 1, bit_value, 7 - bit_pos, client->char_value);
	
	if (client->sig_count % 8 == 0)
	{
		client->msg.message[client->msg_pos] = client->char_value;
		log_msg(LOG_DEBUG, "Character complete: '%c' (ASCII: %d) at position %d/%d",
			client->char_value >= 32 && client->char_value <= 126 ? client->char_value : '?',
			client->char_value, client->msg_pos + 1, client->msg.size_message);
		client->msg_pos++;
		
		if (client->msg_pos >= client->msg.size_message)
			handle_complete_message(client);
	}
}
