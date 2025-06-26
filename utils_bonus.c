/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:03:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 14:16:24 by dlesieur         ###   ########.fr       */
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
	return (1);
}

/**
 * Send 8 bits of a character (LSB first)
 */
void	send_character_bits(t_bclient *client, unsigned char c)
{
	int	i;
	int	bit;
	int	pid;

	pid = client->legacy_client.server_pid;
	i = 0;
	while (i < 8)
	{
		bit = (c >> i) & 1;
		if (bit)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		usleep(100);
		handshake(client);
		i++;
	}
}

void	handshake(t_bclient *client)
{
	while (!client->ack)
		usleep(100);
	client->ack = 0;
}
