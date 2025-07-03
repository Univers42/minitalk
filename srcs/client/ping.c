/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 02:15:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 03:27:46 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	ping_handler(int signum, siginfo_t *info, void *context)
{
	t_server_state	*server;
	const char		*signal_name;

	(void)context;
	server = get_server_instance();
	if (signum == SIGUSR1)
		signal_name = "SIGUSR1 (READY)";
	else
		signal_name = "SIGUSR2 (BUSY)";
	log_msg(LOG_DEBUG, "Ping handler received %s from PID %d",
		signal_name, info->si_pid);
	if (info->si_pid == getpid())
	{
		log_msg(LOG_ERROR, "Received signal from own process");
		exit(EXIT_FAILURE);
	}
	if (server->pid != 0 && info->si_pid != server->pid)
	{
		log_msg(LOG_WARNING, "Unexpected PID in ping_handler: %d (expected %d)",
			info->si_pid, server->pid);
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
	setup_ping_signals(&sa, &sigset);
	server->pid = pid;
	server->is_ready = 0;
	if (handle_timeouts(pid))
	{
		ft_printf("Error: Couldn't reach server PID %d\n", pid);
		log_msg(LOG_ERROR, "Server ping timeout");
		return (0);
	}
	ft_printf("Server ready, sending message.\n");
	log_msg(LOG_SUCCESS, "Server connection established");
	return (1);
}
