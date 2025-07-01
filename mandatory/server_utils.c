/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:47:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 12:47:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_client_state	**get_clients(t_client_state **set)
{
	static t_client_state	*clients = NULL;

	if (set)
		clients = *set;
	return (&clients);
}

t_client_state	*get_client_state(pid_t client_pid)
{
	t_client_state	**clients;
	t_client_state	*curr;
	t_client_state	*new;

	clients = get_clients(NULL);
	curr = *clients;
	while (curr)
	{
		if (curr->client_pid == client_pid)
			return (curr);
		curr = curr->next;
	}
	new = malloc(sizeof(t_client_state));
	if (!new)
		return (NULL);
	ft_memset(new, 0, sizeof(t_client_state));
	new->client_pid = client_pid;
	new->buffer_size = BUFFER_SIZE;
	new->buffer = ft_calloc(new->buffer_size, sizeof(char));
	if (!new->buffer)
		return (free(new), NULL);
	new->next = *clients;
	*clients = new;
	return (new);
}

int	expand_client_buffer(t_client_state *state)
{
	t_string	new_buffer;
	t_string	old_buffer;
	size_t		new_size;

	old_buffer = state->buffer;
	new_size = state->buffer_size * 2;
	new_buffer = ft_realloc(state->buffer, new_size, state->buffer_size);
	if (!new_buffer)
		return (0);
	//log_msg(LOG_INFO, "not enough space allocated, new size = %d from %d\n", ft_strlen(new_buffer), ft_strlen(old__buffer));
	state->buffer = new_buffer;
	ft_memset(state->buffer + state->buffer_size, 0,
		new_size - state->buffer_size);
	state->buffer_size = new_size;
	return (1);
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
