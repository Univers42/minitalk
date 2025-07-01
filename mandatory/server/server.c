/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:50:33 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



static void	process_complete_byte(t_client_state *state,
	t_uint8 byte, pid_t client_pid)
{
	char	display_char;

	if (!handle_buffer_expansion(state, client_pid))
		return ;
	state->buffer[state->byte_index] = byte;
	display_char = get_printable_char(byte);
	log_msg(LOG_DEBUG, "Completed byte for PID %d: 0x%02x ('%c') at index %d",
		client_pid, byte, display_char, state->byte_index);
	state->byte_index++;
	if (byte == '\0')
		handle_message_completion(state, client_pid);
}

// Adds a bit (0 or 1) to the byte at the given bit_index (0 to 7), using math
static void	add_bit_to_byte(t_uint8 *byte, int bit_value, int bit_index)
{
	int	multiplier;
	int	i;

	multiplier = 1;
	i = -1;
	while (++i < (7 - bit_index))
		multiplier *= 2;
	*byte += (bit_value * multiplier);
}

static void	handle_signal(int signal, siginfo_t *info, void *context)
{
	t_client_state	*state;
	pid_t			client_pid;
	int				bit_value;

	client_pid = info->si_pid;
	state = get_client_state(client_pid);
	if (!state)
		return (log_msg(LOG_ERROR, "Client PID %d not found", client_pid));
	if (signal == SIGUSR1)
		bit_value = 1;
	else
		bit_value = 0;
	log_msg(LOG_DEBUG, "Bit %d received from PID %d"
		"(bit_index=%d, byte=0x%02x)",
		bit_value, client_pid, state->bit_index, state->byte);
	add_bit_to_byte(&(state->byte), bit_value, state->bit_index);
	state->bit_index++;
	if (state->bit_index == 8)
	{
		process_complete_byte(state, state->byte, client_pid);
		state->byte = 0;
		state->bit_index = 0;
	}
	(kill(client_pid, SIGUSR1), (void)context);
}
