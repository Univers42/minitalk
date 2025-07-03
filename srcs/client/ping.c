/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 02:15:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 15:24:43 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * know if the other machine is online
 * measure how long it tooks to get the signal to the client server
 * check if there's a packet loss is signals are missing
 */
static int	validate_ping_signal(t_server_state *server, siginfo_t *info)
{
	pid_t	my_pid;

	my_pid = getpid();
	if (info->si_pid == my_pid)
	{
		log_msg(LOG_ERROR, "Received signal from own process");
		exit(EXIT_FAILURE);
	}
	if (server->pid != 0 && info->si_pid != server->pid)
	{
		log_msg(LOG_WARNING, "Ignoring unexpected PID: %d (expected %d)",
			info->si_pid, server->pid);
		return (0);
	}
	if (server->transmission_active)
	{
		log_msg(LOG_DEBUG, "Ignoring ping during active transmission");
		return (0);
	}
	return (1);
}

static void	log_ping_signal(int signum, pid_t sender_pid)
{
	const char	*signal_name;

	if (signum == SIGUSR1)
		signal_name = "SIGUSR1 (READY)";
	else
		signal_name = "SIGUSR2 (BUSY)";
	log_msg(LOG_DEBUG, "Ping handler received %s from PID %d",
		signal_name, sender_pid);
}

void	ping_handler(int signum, siginfo_t *info, void *context)
{
	t_server_state	*server;

	(void)context;
	server = get_server_instance();
	log_ping_signal(signum, info->si_pid);
	if (!validate_ping_signal(server, info))
		return ;
	handle_ping_response(signum, server, info->si_pid);
}


static int	send_ping_signal(int pid)
{
	if (kill(pid, SIGUSR1) == -1)
	{
		log_msg(LOG_ERROR, "Failed to send signal to PID %d", pid);
		ft_printf("Error: Failed to send signal to server PID %d\n", pid);
		return (0);
	}
	return (1);
}

static int	validate_process_exists(int pid)
{
	if (kill(pid, 0) == -1)
	{
		log_msg(LOG_ERROR, "Process PID %d is not accessible", pid);
		ft_printf("Error: Server process PID %d not found\n", pid);
		return (0);
	}
	return (1);
}

static void	handle_retry_delay(int attempt)
{
	if (attempt > RETRY_TIMES / 2)
	{
		log_msg(LOG_INFO, "Server appears busy, increasing retry delay");
		usleep(attempt * 500000);
	}
	else
		usleep(attempt * 100000);
}

static int	attempt_ping(int pid, int attempt)
{
	log_ping_attempt(attempt, RETRY_TIMES);
	if (!validate_process_exists(pid))
		return (-1);
	if (!send_ping_signal(pid))
		return (-1);
	if (check_server_and_sleep())
	{
		log_ping_result(attempt, 1);
		return (1);
	}
	log_ping_result(attempt, 0);
	handle_retry_delay(attempt);
	return (0);
}

int	handle_timeouts(int pid)
{
	int	i;
	int	result;

	i = 0;
	log_msg(LOG_INFO, "Starting ping sequence to server PID %d", pid);
	while (++i <= RETRY_TIMES)
	{
		result = attempt_ping(pid, i);
		if (result == 1)
			return (0);
		if (result == -1)
			return (1);
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
	reset_server_state();
	setup_ping_signals(&sa, &sigset);
	server->pid = pid;
	server->is_ready = 0;
	server->ready_to_proceed = 0;
	server->transmission_active = 0;
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
