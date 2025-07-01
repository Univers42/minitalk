/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 13:06:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <errno.h>

/**
 * need to distinguish between client and server because the data
 * will be stored and different, accross calls.
 */
t_client	*get_instance(t_client *set)
{
	static t_client	*client = NULL;

	if (set)
		client = set;
	return (client);
}

// Returns the value (0 or 1) of the bit at bit_index (7 to 0) in c
inline int	get_bit_from_char(char c, int bit_index)
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

void	get_signal_info(int bit_value, t_array sig, t_strings sig_name)
{
	if (bit_value)
	{
		*sig = SIGUSR1;
		*sig_name = "SIGUSR1";
	}
	else
	{
		*sig = SIGUSR2;
		*sig_name = "SIGUSR2";
	}
}

inline char	get_display_char(char c)
{
	if (c >= 32 && c <= 126)
		return (c);
	else
		return ('.');
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
		log_msg(LOG_ERROR, "kill() failed for PID %d: %s",
			client->server_pid, strerror(errno));
		exit(1);
	}
	while (!client->ack)
		usleep(100);
}