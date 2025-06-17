/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:08:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 13:34:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "libft/libft.h"

static volatile int	g_signal_received = 0;

void	sig_handler(int signum)
{
	(void)signum;
	g_signal_received = 1;
}

void	send_char(pid_t server_pid, unsigned char c)
{
	int	i;

	i = 7;
	while (i >= 0)
	{
		g_signal_received = 0;
		if ((c >> i) & 1)
			kill(server_pid, SIGUSR2);
		else
			kill(server_pid, SIGUSR1);
		while (!g_signal_received)
			usleep(10);
		i--;
	}
}

int	main(int argc, char **argv)
{
	pid_t				server_pid;
	struct sigaction	sa;
	char				*message;
	int					i;

	if (argc != 3)
	{
		ft_printf("Usage: %s <server_pid> <message>\n", argv[0]);
		exit(1);
	}
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	server_pid = ft_atoi(argv[1]);
	message = argv[2];
	ft_printf("Client PID: %d\n", getpid());
	ft_printf("Sending message to server (PID: %d)\n", server_pid);
	i = 0;
	while (message[i])
	{
		send_char(server_pid, (unsigned char)message[i]);
		i++;
	}
	send_char(server_pid, '\0');
	ft_printf("Message sent successfully!\n");
	return (0);
}