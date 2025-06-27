/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_temp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:45:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 19:36:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "log.h"

static t_client_state **get_clients(t_client_state **set)
{
	static t_client_state *clients = NULL;
	if (set)
		clients = *set;
	return &clients;
}

static void reset_client_state(t_client_state *state)
{
	state->byte = 0;
	state->bit_index = 0;
	state->byte_index = 0;
	if (state->buffer)
		memset(state->buffer, 0, state->buffer_size);
}

static int expand_client_buffer(t_client_state *state)
{
	size_t new_size = state->buffer_size * 2;
	char *new_buffer = realloc(state->buffer, new_size);
	if (!new_buffer)
		return (0);
	state->buffer = new_buffer;
	memset(state->buffer + state->buffer_size, 0, new_size - state->buffer_size);
	state->buffer_size = new_size;
	return (1);
}

static t_client_state *get_client_state(pid_t client_pid)
{
	t_client_state **clients = get_clients(NULL);
	t_client_state *curr = *clients;
	while (curr)
	{
		if (curr->client_pid == client_pid)
			return curr;
		curr = curr->next;
	}
	// Not found, create new
	t_client_state *new = malloc(sizeof(t_client_state));
	if (!new)
		return NULL;
	new->client_pid = client_pid;
	new->byte = 0;
	new->bit_index = 0;
	new->byte_index = 0;
	new->buffer_size = BUFFER_SIZE;
	new->buffer = calloc(new->buffer_size, sizeof(char));
	if (!new->buffer)
	{
		free(new);
		return NULL;
	}
	new->next = *clients;
	*clients = new;
	return new;
}

static void free_all_clients(void)
{
	t_client_state **clients = get_clients(NULL);
	t_client_state *curr = *clients;
	while (curr)
	{
		t_client_state *next = curr->next;
		if (curr->buffer)
			free(curr->buffer);
		free(curr);
		curr = next;
	}
	*clients = NULL;
}

static void process_complete_byte(t_client_state *state, unsigned char byte, pid_t client_pid)
{
	// Expand buffer if needed
	if ((size_t)state->byte_index >= state->buffer_size - 1)
	{
		if (!expand_client_buffer(state))
		{
			log_msg(LOG_ERROR, "Memory reallocation failed for client %d", client_pid);
			kill(client_pid, SIGUSR2);
			reset_client_state(state);
			return;
		}
	}
	state->buffer[state->byte_index] = byte;
	state->byte_index++;
	if (byte == '\0')
	{
		log_msg(LOG_SUCCESS, "Received message from PID %d: '%s'", client_pid, state->buffer);
		kill(client_pid, SIGUSR1);
		reset_client_state(state);
	}
}

// Adds a bit (0 or 1) to the byte at the given bit_index (0 to 7), using math
static void add_bit_to_byte(unsigned char *byte, int bit_value, int bit_index)
{
	int multiplier = 1;
	for (int i = 0; i < 7 - bit_index; ++i)
		multiplier *= 2;
	*byte += bit_value * multiplier;
}

void handle_signal(int signal, siginfo_t *info, void *context)
{
	pid_t client_pid = info->si_pid;
	t_client_state *state = get_client_state(client_pid);
	if (!state)
	{
		log_msg(LOG_ERROR, "Client PID %d not found", client_pid);
		return;
	}
	int bit_value = (signal == SIGUSR1) ? 1 : 0;
	log_msg(LOG_DEBUG, "Bit %d received from PID %d", bit_value, client_pid);
	add_bit_to_byte(&(state->byte), bit_value, state->bit_index);
	state->bit_index++;
	if (state->bit_index == 8)
	{
		process_complete_byte(state, state->byte, client_pid);
		state->byte = 0;
		state->bit_index = 0;
	}
	kill(client_pid, SIGUSR1);
	(void)context;
}

void init_server(void)
{
	log_msg(LOG_INFO, "Server started on PID %d", getpid());
}

int main(void)
{
	struct sigaction sa;
	t_client_state *clients = NULL;

	get_clients(&clients); // <-- Set the static pointer
	init_server();
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		log_msg(LOG_ERROR, "Error setting up SIGUSR1 handler");
		return (1);
	}
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		log_msg(LOG_ERROR, "Error setting up SIGUSR2 handler");
		return (1);
	}
	log_msg(LOG_INFO, "Server ready, waiting for messages...");
	while (1)
		pause();
	free_all_clients();
	return (0);
}