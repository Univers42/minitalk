/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:49:15 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 10:49:18 by dlesieur         ###   ########.fr       */
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

int	handle_buffer_expansion(t_client_state *state, pid_t client_pid)
{
	if ((size_t)state->byte_index >= state->buffer_size - 1)
	{
		if (!expand_client_buffer(state))
		{
			log_msg(LOG_ERROR, "Memory reallocation failed for client %d",
				client_pid);
			kill(client_pid, SIGUSR2);
			reset_client_state(state);
			return (0);
		}
	}
	return (1);
}