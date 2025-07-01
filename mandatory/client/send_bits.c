/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_bits.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:06:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:06:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void	send_byte(t_client *client, char c)
{
	int	b_seq;

	b_seq = BYTE_SIZE;
	while (b_seq--)
		send_bit(client, c, b_seq);
}

// Sends the message, including the null terminator
static void	send_message(t_client *client)
{
	t_string str;

	str = client->message;
	while (*str)
		send_byte(client, *str++);
	send_byte(client, '\0');
}

inline void	send_bit(t_client *client, char c, int bit)
{
	t_string	sig_name;
	char		display_char;
	int			bit_value;
	int			sig;

	client->ack = 0;
	bit_value = get_bit_from_char(c, bit);
	get_signal_info(bit_value, &sig, &sig_name);
	display_char = get_display_char(c);
	log_msg(LOG_DEBUG, "Sending bit %d (sig=%s) for char"
		"'%c' (0x%02x) to PID %d",
		bit_value, sig_name, display_char, (unsigned char)c,
		client->server_pid);
	if (kill(client->server_pid, sig) == -1)
	{
		log_msg(LOG_ERROR, "kill() failed for PID %d",
			client->server_pid);
		exit(1);
	}
	while (!client->ack)
		usleep(100);
}

// Returns the value (0 or 1) of the bit at bit_index (7 to 0) in c
int	get_bit_from_char(char c, int bit_index)
{
	int	value;
	int	divisor;
	int	i;

	value = (unsigned char)c;
	divisor = 1;
	i = -1;
	while (++i < bit_index)
		divisor *= 2;
	return ((value / divisor) % 2);
}

inline char	get_display_char(char c)
{
	if (c >= 32 && c <= 126)
		return (c);
	else
		return ('.');
}