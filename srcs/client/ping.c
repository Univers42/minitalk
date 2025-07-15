/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 02:15:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 03:24:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * What is a ping in system environment ?
 * know if the other machine is online
 * measure how long it tooks to get the signal to the client server
 * check if there's a packet loss is signals are missing
 * 
 * all the ping system is used just to check the connection
 * between the state server and the client
 * it's true that server can be busy so we need to know that
 * once the client is allow to pass its message the pind has no use.
 */

/**
 * @param signum signal identificator we use
 * @param info ptr to sig_info_t struct
 * @param context useless
 * @note check if the signal are sent correcltly
 */
void	ping_handler(int signum, siginfo_t *info, void *context)
{
	t_server_state	*server;

	(void)context;
	server = get_server_instance();
	log_ping_signal(signum, info->si_pid);
	if (!validate_ping_signal(server, info))
		return ;
	handle_pong(signum, server, info->si_pid);
}

/**
 * @param pid process identification
 * @return 1 | 0 depending on error or not
 */
int	send_ping_signal(int pid)
{
	if (kill(pid, SIGUSR1) == -1)
	{
		log_msg(LOG_ERROR, "Failed to send signal to PID %d", pid);
		ft_printf("Error: Failed to send signal to server PID %d\n", pid);
		return (0);
	}
	return (1);
}

/**
 * 1. create a connectin
 * !okay : send error
 * 2. send ping 
 * try several times to connect
 * and if it doensn't work the dealy is triggered
 * 
 */
int	attempt_ping(int pid, int attempt)
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

/**
 * the ping work with the server and check if
 * it is ready to receive the message
 * check the boolean to send one by one the different
 * client
 * it works like a semaphore system
 * red = is_ready ?
 * orange = ready_to_proceed
 * green = transmission active
 * while the transmision is active we handle a timeout
 */
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
