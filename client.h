/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:43:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 12:12:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H
# include "libft.h"

# define BYTE_SIZE 8

typedef struct s_client
{
	pid_t		server_pid;
	char		*message;
	bool		await;
	pid_t		client_pid;
}				t_client;

void	send_message(t_client *client);
void	handle_signal(int signal, siginfo_t *info, void *context);
int		init_client(t_client *client, int argc, char **argv);

#endif