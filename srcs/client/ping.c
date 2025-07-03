/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 02:15:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 04:33:51 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	ping_handler(int signum, siginfo_t *info, void *context)
{
	t_server_state	*server;
	const char		*signal_name;
	pid_t			my_pid;

	(void)context;
	server = get_server_instance();
	my_pid = getpid();
	
	if (signum == SIGUSR1)
		signal_name = "SIGUSR1 (READY)";
	else
		signal_name = "SIGUSR2 (BUSY)";
	
	log_msg(LOG_DEBUG, "Ping handler received %s from PID %d",
		signal_name, info->si_pid);
	
	if (info->si_pid == my_pid)
	{
		log_msg(LOG_ERROR, "Received signal from own process");
		exit(EXIT_FAILURE);
	}
	
	// Only process ping responses from the server we're trying to connect to
	if (server->pid != 0 && info->si_pid != server->pid)
	{
		log_msg(LOG_WARNING, "Ignoring signal from unexpected PID: %d (expected %d)",
			info->si_pid, server->pid);
		return ;
	}
	
	// Don't process ping responses if we're already in transmission mode
	if (server->transmission_active)
	{
		log_msg(LOG_DEBUG, "Ignoring ping response during active transmission");
		return ;
	}
	
	handle_ping_response(signum, server, info->si_pid);
}

int	check_server_and_sleep(void)
{
	const int		total_sleep = RETRY_TIME * 1000 * 1000;
	const int		sleep_time = total_sleep / RETRY_INTERVALS;
	int				i;
	t_server_state	*server;

	server = get_server_instance();
	i = 0;
	while (i <= total_sleep)
	{
		usleep(sleep_time);
		if (server->is_ready == 1)
			return (1);
		i += sleep_time;
	}
	return (0);
}

int	handle_timeouts(int pid)
{
	int	i;

	i = 0;
	log_msg(LOG_INFO, "Starting ping sequence to server PID %d", pid);
	while (++i <= RETRY_TIMES)
	{
		log_ping_attempt(i, RETRY_TIMES);
		kill(pid, SIGUSR1);
		if (check_server_and_sleep())
		{
			log_ping_result(i, 1);
			return (0);
		}
		log_ping_result(i, 0);
		// Exponential backoff for retries
		usleep(i * 100000); // Additional delay between retries
	}
	log_msg(LOG_ERROR, "Server did not respond after %d attempts", RETRY_TIMES);
	return (1);
}

int	ping(int pid)
{
	struct sigaction	sa;
	sigset_t			sigset;
	t_server_state		*server;

	server = get_server_instance();
	
	// Reset state completely before starting
	reset_server_state();
	
	setup_ping_signals(&sa, &sigset);
	server->pid = pid;
	server->is_ready = 0;
	server->ready_to_proceed = 0;
	server->transmission_active = 0; // Reset transmission state
	
	if (handle_timeouts(pid))
	{
		ft_printf("Error: Couldn't reach server PID %d\n", pid);
		log_msg(LOG_ERROR, "Server ping timeout");
		return (0);
	}
	
	ft_printf("Server ready, waiting for transmission slot...\n");
	log_msg(LOG_SUCCESS, "Server connection established");
	return (1);
}
