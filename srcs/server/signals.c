/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/15 04:43:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

/**
 * same here I refactorized eleminiting redundancies
 * all member added to 0 when it was unnecessary
 */
int	pong(int pid)
{
	t_client_state	*client;

	client = get_client_instance();
	if (is_server_busy() && client->actual_pid != pid)
	{
		log_msg(LOG_INFO, "Server busy, sending busy signal to client %d", pid);
		kill(pid, SERVER_BUSY);
		return (EXIT_SUCCESS);
	}
	ft_memset(client, 0, sizeof(*client));
	client->actual_pid = pid;
	client->client_pid = pid;
	client->getting_header = 1;
	client->client_activity = 1;
	set_server_busy(pid);
	log_msg(LOG_INFO, "Client %d connected, sending ready signal", pid);
	kill(pid, SERVER_READY);
	log_msg(LOG_SUCCESS, "Server ready signal sent to client %d", pid);
	return (EXIT_SUCCESS);
}

void	send_multiple_acks(pid_t client_pid)
{
	kill(client_pid, SIGUSR2);
	log_msg(LOG_DEBUG, "Sent acknowledgment signal to client %d", client_pid);
}

int	lost_signal(int s_si_pid, int signum, void *context)
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

void	send_completion_signal(pid_t client_pid)
{
	if (client_pid > 0)
	{
		log_msg(LOG_INFO, "Sending completion signal to client %d",
			client_pid);
		kill(client_pid, SIGUSR1);
	}
}
