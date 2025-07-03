/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:17:43 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 04:34:21 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

t_server_state	*get_server_instance(void)
{
	static t_server_state	server = {0, 0, 0, 0, 0, 0};

	return (&server);
}

void	reset_server_state(void)
{
	t_server_state	*server;

	server = get_server_instance();
	server->pid = 0;
	server->is_ready = 0;
	server->ready_to_proceed = 0;
	server->transmission_active = 0;
	server->current_client_pid = 0;
	server->transmission_id = 0;
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

int	is_transmission_owner(pid_t client_pid)
{
	t_server_state	*server;

	server = get_server_instance();
	return (server->transmission_active && server->current_client_pid == client_pid);
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