/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_message.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:24:27 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 18:24:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_complete_message(t_client_state *client)
{
	pid_t	client_pid;

	client->msg.message[client->msg_pos] = '\0';
	log_msg(LOG_SUCCESS, "Message reception complete:"
		"%d characters received", client->msg_pos);
	print_message(client);
	client_pid = client->client_pid;
	if (client->msg.message)
	{
		free(client->msg.message);
		client->msg.message = NULL;
	}
	send_completion_signal(client_pid);
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
		log_msg(LOG_DEBUG, "Character complete: '%c'"
			" (ASCII: %d) at position %d/%d",
			client->char_value, client->char_value, client->msg_pos + 1,
			client->msg.size_message);
		client->msg_pos++;
		client->char_value = 0;
		if (client->msg_pos >= client->msg.size_message)
			handle_complete_message(client);
	}
}

// Set the bit in the correct position (MSB first: 7, 6, 5, ..., 0)
void	handle_msg(int signum)
{
	t_client_state	*client;
	const int		bit_value = get_bit_value(signum);
	int				bit_pos;

	client = get_client_instance();
	bit_pos = client->sig_count % 8;
	if (bit_pos == 0)
	{
		client->char_value = 0;
		log_msg(LOG_DEBUG, "Starting new character %d/%d",
			client->msg_pos + 1, client->msg.size_message);
	}
	if (bit_value == 1)
		client->char_value |= (1 << (7 - bit_pos));
	client->sig_count++;
	log_msg(LOG_DEBUG, "Bit %d/8: %d (bit_pos: %d, char value: %d)",
		bit_pos + 1, bit_value, 7 - bit_pos, client->char_value);
	if (client->sig_count % 8 == 0)
	{
		client->msg.message[client->msg_pos] = client->char_value;
		log_character_completion(client);
		client->msg_pos++;
		if (client->msg_pos >= client->msg.size_message)
			handle_complete_message(client);
	}
}
