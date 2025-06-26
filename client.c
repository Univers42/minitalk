/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:08:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 13:00:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * @bried Send a message to the server by sending signals. 
 * Each character of the message is sent using a series of signals,,
 * 
 */
void	send_message(t_client *client)
{
	char	c;
	int		bit_index;

	client->await = true;
	while (client->await)
	{
		c = *client->message++;
		bit_index = BYTE_SIZE;
		while (bit_index--)
		{
			if (c & (1 << bit_index))
				kill(client->server_pid, SIGUSR1);
			else
				kill(client->server_pid, SIGUSR2);
			usleep(100);
		}
		if (c == '\0')
			client->await = false;
	}
}

void	handle_signal(int signal, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (signal == SIGUSR1)
		ft_printf("Signal SIGUSR1 received from PID %d\n", info->si_pid);
	else if (signal == SIGUSR2)
		ft_printf("Signal SIGUSR2 received from PID %d\n", info->si_pid);
	printf("Received signal %d from PID %d\n", signal, info->si_pid);
}

int	init_client(t_client *client, int argc, char **argv)
{
	ft_memset(client, 0, sizeof(t_client));
	if (argc < 3)
		return (ft_printf("Usage: %s <server_pid> <message>\n", *argv), 1);
	argv++;
	client->server_pid = ft_atoi(*argv++);
	if (client->server_pid <= 0)
		return (ft_printf("Invalid server PID: %s\n", *(argv - 1)), 1);
	client->message = *argv;
	if (!client->message || !*client->message)
		return (ft_printf("No message provided\n"), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_client	client;
	t_sigaction	sa;

	init_client(&client, argc, argv);
	client.client_pid = getpid();
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (ft_printf("Error setting up signal handler\n"), 1);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (ft_printf("Error setting up signal handler\n"), 1);
	ft_printf("Client PID: %d\n", client.client_pid);
	send_message(&client);
	client.await = true;
	return (0);
}
