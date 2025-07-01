/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 10:45:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "libft.h"
# include "../minitalk.h"

# define BYTE_SIZE	8

typedef struct s_client
{
	pid_t	server_pid;
	char	*message;
	bool	await;
	pid_t	client_pid;
	int		ack;
}			t_client;

void		send_message(t_client *client);
void		handle_signal(int signal, siginfo_t *info, void *context);
int			init_client(t_client *client, int argc, t_strings argv);
t_client	*get_instance(t_client *set);
int			get_bit_from_char(char c, int bit_index);
void		get_signal_info(int bit_value, int *sig, t_strings sig_name);
char		get_display_char(char c);
void		send_bit(t_client *client, char c, int bit);

#endif