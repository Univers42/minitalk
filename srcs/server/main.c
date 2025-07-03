/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/03 10:36:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	lost_signal(int s_si_pid, int signum, void *context)
{
	t_client_state	*client;

	(void)context;
	client = get_client_instance();
	if (s_si_pid == 0 && (signum == SIGUSR1 || signum == SIGUSR2))
	{
		ft_printf("client: %d with signal: %d\n", s_si_pid, signum);
		s_si_pid = client->actual_pid;
	}
	return (s_si_pid);
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	t_client_state	*client;

	client = get_client_instance();
	info->si_pid = lost_signal(info->si_pid, signum, context);
	if (info->si_pid == getpid())
		return ;
	
	// Handle new client connection (ping) when server is idle
	if (client->actual_pid == 0)
	{
		log_msg(LOG_INFO, "New client connection from PID %d", info->si_pid);
		client->client_pid = info->si_pid;
		pong(client->client_pid);
		return ;
	}
	
	// If server is busy with another client, reject the signal immediately
	if (client->actual_pid != info->si_pid)
	{
		log_msg(LOG_WARNING, "Server busy, rejecting signal from PID %d (current client: %d)", 
			info->si_pid, client->actual_pid);
		kill(info->si_pid, SERVER_BUSY);
		return ;
	}
	
	// Process signals only from the current active client
	client->client_pid = info->si_pid;
	client->client_activity = 1;
	log_msg(LOG_DEBUG, "Processing signal %d from active client %d", 
		signum, client->client_pid);
	
	// Simple routing based on state
	if (client->getting_msg == 1)
		handle_msg(signum);
	else if (client->getting_header == 1)
		handle_header(signum);
	
	// Send single acknowledgment signal
	send_multiple_acks(client->client_pid);
}

int	main(void)
{
	struct sigaction	sa;
	sigset_t			sigset;
	pid_t				server_pid;

	server_pid = getpid();
	ft_printf("Server PID: %d\n", server_pid);
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sa.sa_mask = sigset;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	keep_server_up();
	return (0);
}
