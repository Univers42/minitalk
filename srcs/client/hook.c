/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:21:00 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 03:27:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

//just a wrapper that help to trace the error if occuring
void	establish_connection(t_client *data)
{
	if (!connect_to_server(data))
	{
		ft_printf("Failed to connect to server\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * works with all the ping system
 */
int	connect_to_server(t_client *data)
{
	log_msg(LOG_INFO, "Attempting to connect to server PID %d",
		data->server_pid);
	if (kill(data->server_pid, 0) == -1)
	{
		ft_printf("Error: Server process PID %d not accessible\n",
			data->server_pid);
		log_msg(LOG_ERROR, "Server process validation failed");
		return (0);
	}
	if (ping(data->server_pid) == 0)
		return (0);
	return (1);
}

void	start_transmission(t_client *data, int msg_len)
{
	wait_for_transmission_slot(data);
	ft_printf("Starting transmission (%d characters)...\n", msg_len);
	log_msg(LOG_INFO, "Starting header transmission (message length)");
	send_signals(&msg_len, 32, data);
	log_msg(LOG_INFO, "Header transmission complete, starting message content");
	send_message(data->msg, data);
	log_msg(LOG_INFO, "All data transmitted, waiting for final confirmation");
}

void	set_transmission_active(pid_t client_pid)
{
	t_server_state	*server;

	server = get_server_instance();
	server->transmission_active = 1;
	server->current_client_pid = client_pid;
	server->transmission_id++;
	log_msg(LOG_INFO, "Transmission activated for client PID %d (ID: %d)",
		client_pid, server->transmission_id);
}

void	end_transmission(void)
{
	t_server_state	*server;

	server = get_server_instance();
	log_msg(LOG_INFO, "Ending transmission for client PID %d",
		server->current_client_pid);
	server->transmission_active = 0;
	server->current_client_pid = 0;
	server->ready_to_proceed = 0;
}
