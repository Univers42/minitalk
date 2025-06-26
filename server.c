/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:45:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/25 23:41:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"


void	decode_msg(t_server *srv)
{
	char c;
	srv->await = true;
	srv->bit_index = 0;
	srv->byte_index = 0;

	while (srv->await)
	{
		c = srv->buffer[srv->byte_index];
		srv->bit_index = 8;
		while (srv->bit_index--)
		{
			c = (c << 1) | (srv->buffer[srv->byte_index] & 1);
			srv->buffer[srv->byte_index] >>= 1;
		}
		if (c == '\0')
		{
			ft_printf("Received message: %s\n", srv->buffer);
			srv->byte_index = 0;
			ft_bzero(srv->buffer, BUFFER_SIZE);
		}
	}
}

t_server *get_srv_instance(t_server *set)
{
	static t_server *srv = NULL;
	if (set)
		srv = set;
	return (srv);
}

void handle_signal(int signal, siginfo_t *info, void *context)
{
	static unsigned char byte = 0;
	static int bit_index = 0;
	static int byte_index = 0;
	t_server *srv = get_srv_instance(NULL);
	(void)context;
	(void)info;

	byte <<= 1;
	if (signal == SIGUSR1)
		byte |= 1;
	bit_index++;
	if (bit_index == 8)
	{
		if (byte_index < BUFFER_SIZE - 1)
		{
			srv->buffer[byte_index++] = byte;
			if (byte == '\0')
			{
				ft_printf("Received message: %s\n", srv->buffer);
				byte_index = 0;
				ft_bzero(srv->buffer, BUFFER_SIZE);
			}
		}
		else
		{
			ft_printf("Buffer overflow, message too long\n");
			byte_index = 0;
			ft_bzero(srv->buffer, BUFFER_SIZE);
		}
		byte = 0;
		bit_index = 0;
	}
}

void init_server(t_server *srv)
{
	ft_memset(srv, 0, sizeof(t_server));
	srv->pid = getpid();
	ft_printf("Server PID: %d\n", srv->pid);
	srv->buffer = ft_calloc(BUFFER_SIZE, sizeof(char));
	if (!srv->buffer)
	{
		ft_printf("Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
}

int main(void)
{
	t_server	srv;
	t_sigaction sa;

	init_server(&srv);
	get_srv_instance(&srv);
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		ft_printf("Error setting up signal handler\n");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_printf("Error setting up signal handler\n");
		exit(EXIT_FAILURE);
	}
	srv.await = true;
	while(srv.await)
		pause();
	return (0);
}