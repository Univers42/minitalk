/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:29:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "libft.h"
# include "../minitalk.h"

# define BYTE_SEQ		8
# define SHAKE_HANDED	1

typedef struct s_client
{
	t_pid		server_pid;
	t_pid		client_pid;
	t_string	message;
	bool		await;
	t_flag		ack;
	t_flag		ready;
}				t_client;

t_client	*get_instance(t_client *set);
int			get_bit_from_char(char c, int bit_index);
void		get_signal_info(int bit_value, int *sig, t_strings sig_name);
char		get_display_char(char c);
void		send_bit(t_client *client, char c, int bit);
int			is_pid_alive(pid_t pid);
int			is_valid_pid(const char *str);
#endif