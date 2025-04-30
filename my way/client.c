/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:15:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 23:28:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minitalk.h"

struct timeval	g_start_time;

void	send_char(pid_t pid, char c)
{
	int	bit;

	bit = 0;
	while (bit < 8)
	{
		if ((c & (1 << bit)) != 0)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		usleep(SIGNAL_DELAY);
		bit++;
	}
}

void	send(pid_t pid, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		send_char(pid, str[i]);
		i++;
	}
	send_char(pid, '\0');
}

void	sigusr1_handler(int sig)
{
	struct timeval	end_time;
	double			elapsed_time;

	(void)sig;
	gettimeofday(&end_time, NULL);
	elapsed_time = (end_time.tv_sec - g_start_time.tv_sec) * 1000.0;
	elapsed_time += (end_time.tv_usec - g_start_time.tv_usec) / 1000.0;
	printf("Message transmitted and received in %.3f milliseconds\n", elapsed_time);
	exit(0);
}

int	main(int argc, char **argv)
{
	pid_t				pid;
	char				*message;
	struct sigaction	sa;

	if (argc != 3)
	{
		printf("Usage: %s <PID> <message>\n", argv[0]);
		return (1);
	}
	pid = atoi(argv[1]);
	if (pid <= 0)
	{
		printf("Invalid PID\n");
		return (1);
	}
	message = argv[2];
	sa.sa_handler = sigusr1_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	printf("Sending message (%lu chars) to server (PID: %d)...\n", 
		strlen(message), pid);
	gettimeofday(&g_start_time, NULL);
	send(pid, message);
	while (1)
		pause();
	return (0);
}
