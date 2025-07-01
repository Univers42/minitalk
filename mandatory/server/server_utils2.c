/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:49:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 13:10:12 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_message_completion(t_client_state *state, pid_t client_pid)
{
	ft_printf("Received message from PID %d: '%s'\n",
		client_pid, state->buffer);
	kill(client_pid, SIGUSR1);
	reset_client_state(state);
}


void	reset_client_state(t_client_state *state)
{
	state->byte = 0;
	state->bit_index = 0;
	state->byte_index = 0;
	if (state->buffer)
		ft_memset(state->buffer, 0, state->buffer_size);
}

void	free_all_clients(void)
{
	t_client_state	**clients;
	t_client_state	*curr;
	t_client_state	*next;

	clients = get_clients(NULL);
	curr = *clients;
	while (curr)
	{
		next = curr->next;
		if (curr->buffer)
			free(curr->buffer);
		free(curr);
		curr = next;
	}
	*clients = NULL;
}

inline char	get_printable_char(t_uint8 byte)
{
	if (ft_isprint(byte))
		return (byte);
	else
		return ('.');
}