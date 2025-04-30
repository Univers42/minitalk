/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:51:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 23:36:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minitalk.h"
#include <sys/time.h>

t_data	g_data;

void	reset_data(pid_t client_pid)
{
	g_data.c = 0;
	g_data.bit_index = 0;
	g_data.client_pid = client_pid;
	g_data.char_count = 0;
	gettimeofday(&g_data.start_time, NULL);
}

void	handle_complete_char(void)
{
	double	elapsed;
	
	if (g_data.c == '\0')
	{
		write(1, "\n", 1);
		gettimeofday(&g_data.end_time, NULL);
		elapsed = (g_data.end_time.tv_sec - g_data.start_time.tv_sec) * 1000.0;
		elapsed += (g_data.end_time.tv_usec - g_data.start_time.tv_usec) / 1000.0;
		printf("Received %d characters in %.3f milliseconds (%.2f chars/sec)\n", 
			g_data.char_count, elapsed, g_data.char_count * 1000.0 / elapsed);
		fflush(stdout);
		kill(g_data.client_pid, SIGUSR1);
	}
	else
	{
		write(1, &g_data.c, 1);
		g_data.char_count++;
	}
	g_data.c = 0;
	g_data.bit_index = 0;
}

void	signal_handler(int sig, siginfo_t *info, void *context)
{
	(void)context;
	if (g_data.client_pid != info->si_pid)
		reset_data(info->si_pid);
	if (sig == SIGUSR2)
		g_data.c |= (1 << g_data.bit_index);
	g_data.bit_index++;
	if (g_data.bit_index == 8)
		handle_complete_char();
}

int	main(void)
{
	pid_t				pid;
	struct sigaction	sa;

	pid = getpid();
	printf("Server PID: %d\n", pid);
	fflush(stdout); // Force output to be written immediately
	g_data.c = 0;
	g_data.bit_index = 0;
	g_data.client_pid = 0;
	g_data.char_count = 0;
	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	printf("Waiting for messages...\n");
	fflush(stdout); // Flush this message too
	while (1)
		pause();
	return (0);
}
