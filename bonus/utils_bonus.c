/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 21:23:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_bonus.h"
#include "client_bonus.h"

t_server	*get_server(void)
{
	static t_server	server;

	return (&server);
}

t_bclient	*get_client_instance(t_bclient *set)
{
	static t_bclient	*client;

	if (set)
		client = set;
	return (client);
}

int	initialize_client(t_bclient *client, int argc, char **argv)
{
	ft_memset(client, 0, sizeof(t_bclient));
	if (argc != 3)
		return (0);
	client->legacy_client.server_pid = ft_atoi(argv[1]);
	client->legacy_client.message = argv[2];
	client->ack = 0;
	client->timeout = 0;
	return (1);
}

void	cleanup_still_reachable(int sig)
{
	t_server	*srv;

	(void)sig;
	srv = get_server();
	if (srv->msg_buf)
		free(srv->msg_buf);
	exit(0);
}
