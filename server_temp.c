/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:45:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 13:25:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void	reset_buffer(t_server *srv, int *byte_index)
{
	*byte_index = 0;
	ft_bzero(srv->buffer, srv->buffer_size);
}

static int	expand_buffer_if_needed(t_server *srv, int byte_index)
{
	char	*new_buffer;
	size_t	new_size;

	if ((size_t)byte_index < srv->buffer_size - 1)
		return (1);
	
	new_size = srv->buffer_size * 2;
	new_buffer = ft_realloc(srv->buffer, srv->buffer_size, new_size);
	if (!new_buffer)
	{
		ft_printf("Memory reallocation failed\n");
		return (0);
	}
	srv->buffer = new_buffer;
	srv->buffer_size = new_size;
	ft_printf("Buffer expanded to %zu bytes\n", new_size);
	return (1);
}

static void	process_complete_byte(unsigned char byte, int *byte_index,
		pid_t client_pid)
{
	t_server	*srv;

	srv = get_srv_instance(NULL);
	if (!srv)
		return ;
	
	// Expand buffer if needed
	if (!expand_buffer_if_needed(srv, *byte_index))
	{
		ft_printf("Failed to expand buffer, message truncated\n");
		kill(client_pid, SIGUSR2);
		reset_buffer(srv, byte_index);
		return ;
	}
	
	srv->buffer[*byte_index] = byte;
	(*byte_index)++;
	
	if (byte == '\0')
	{
		ft_printf("Received: %s\n", srv->buffer);
		kill(client_pid, SIGUSR1);
		reset_buffer(srv, byte_index);
	}
}

void	handle_signal(int signal, siginfo_t *info, void *context)
{
	static unsigned char	byte = 0;
	static int				bit_index = 0;
	static int				byte_index = 0;

	(void)context;
	if (signal == SIGUSR1)
		byte = (byte << 1) | 1;
	else if (signal == SIGUSR2)
		byte = byte << 1;
	bit_index++;
	if (bit_index == 8)
	{
		process_complete_byte(byte, &byte_index, info->si_pid);
		byte = 0;
		bit_index = 0;
	}
}

void	init_server(t_server *srv)
{
	ft_memset(srv, 0, sizeof(t_server));
	srv->pid = getpid();
	ft_printf("Server PID: %d\n", srv->pid);
	srv->buffer_size = BUFFER_SIZE;
	srv->buffer = ft_calloc(srv->buffer_size, sizeof(char));
	if (!srv->buffer)
	{
		ft_printf("Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	srv->await = 1;
}

int	main(void)
{
	t_server			srv;
	struct sigaction	sa;

	init_server(&srv);
	get_srv_instance(&srv);
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		ft_printf("Error setting up SIGUSR1 handler\n");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_printf("Error setting up SIGUSR2 handler\n");
		exit(EXIT_FAILURE);
	}
	ft_printf("Server ready, waiting for messages...\n");
	while (srv.await)
		pause();
	free(srv.buffer);
	return (0);
}