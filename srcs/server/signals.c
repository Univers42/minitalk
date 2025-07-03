/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/03 10:36:09 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

#define SERVER_READY SIGUSR1
#define SERVER_BUSY SIGUSR2

int	pong(int pid)
{
	t_client_state	*client;

	client = get_client_instance();
	
	// Check if server is already busy with another client
	if (is_server_busy() && client->actual_pid != pid)
	{
		log_msg(LOG_INFO, "Server busy, sending busy signal to client %d", pid);
		kill(pid, SERVER_BUSY);
		return (EXIT_SUCCESS);
	}
	
	// Accept the connection and set up client state
	client->actual_pid = pid;
	client->client_pid = pid;
	client->getting_header = 1;
	client->getting_msg = 0;
	client->sig_count = 0;
	client->msg_pos = 0;
	client->char_value = 0;
	client->client_activity = 1;
	
	// Set server as busy with this client
	set_server_busy(pid);
	
	log_msg(LOG_INFO, "Client %d connected, sending ready signal", pid);
	kill(pid, SERVER_READY);
	log_msg(LOG_SUCCESS, "Server ready signal sent to client %d", pid);
	return (EXIT_SUCCESS);
}

void	send_multiple_acks(pid_t client_pid)
{
	// Send only one acknowledgment signal - simple and reliable
	kill(client_pid, SIGUSR2);
	log_msg(LOG_DEBUG, "Sent acknowledgment signal to client %d", client_pid);
}