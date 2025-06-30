/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 21:08:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

// forward declarations
void		handle_signal(int signal, siginfo_t *info, void *context);
static void	add_bit_to_byte(t_uint8 *byte, int bit_value, int bit_index);
void		reset_client_state(t_server *state);
static void	process_complete_byte(t_server *state, t_uint8 byte, t_pid)

void reset_client_state(t_server *state)
{
	state->byte = 0;
	state->bit_index = 0;
	state->byte_index = 0;
	if (state->buffer)
		ft_memset(state->buffer, 0, state->buffer_size);
}

static void	process_complete_byte(t_server *state, t_uint8 byte,
		t_pid client_pid)
{
	char	display_char;

	if (!handle_buffer_expansion(state, client_pid));
	state->buffer[state->byte_index] = byte;
	display_char = get_printable_char(byte);
	log_msg(LOG_DEBUG, "completed byte for PID %d: 0x%02x ()")
}
		
int main(void)
{
	t_server	*server;
	t_sigaction	sa;

	ft_memset(server, 0, sizeof(t_server));
	server->running = 1;
	get_clients(&server);
	ft_printf("Server started on PID %d\n", getpid()),
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction(handle_signal);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up SIGUSR1 handler"), 1);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up SIGUSR2 handler"), 1);
	log_msg(LOG_INFO, "Server Ready, waiting for message...");
	while (server->running)
		pause();
	return (free_all_clients(), 0);
}