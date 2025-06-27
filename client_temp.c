/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:45:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 13:52:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static volatile int	g_received = 0;

void	handle_signal(int signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signal == SIGUSR1)
		g_received = 1;
}

void	send_message(t_client *client)
{
	char	*str = client->message;
	int		bit;

	while (*str)
	{
		bit = 7;
		while (bit >= 0)
		{
			if ((*str >> bit) & 1)
				kill(client->server_pid, SIGUSR1);
			else
				kill(client->server_pid, SIGUSR2);
			usleep(500);
			bit--;
		}
		str++;
	}
	// Send null terminator
	bit = 7;
	while (bit >= 0)
	{
		kill(client->server_pid, SIGUSR2);
		usleep(500);
		bit--;
	}
}

int	init_client(t_client *client, int argc, char **argv)
{
	ft_memset(client, 0, sizeof(t_client));
	if (argc != 3)
		return (ft_printf("Usage: %s <server_pid> <message>\n", argv[0]), 1);
	client->server_pid = ft_atoi(argv[1]);
	if (client->server_pid <= 0)
		return (ft_printf("Invalid server PID: %s\n", argv[1]), 1);
	client->message = argv[2];
	return (0);
}

int	main(int argc, char **argv)
{
	t_client			client;
	struct sigaction	sa;

	if (init_client(&client, argc, argv) != 0)
		return (1);
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (ft_printf("Error setting up signal handler\n"), 1);
	send_message(&client);
	while (!g_received)
		usleep(100);
	return (0);
}