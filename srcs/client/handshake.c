/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handshake.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:19:47 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:20:18 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	handle_acknowledgment(t_server_state *server)
{
	server->ready_to_proceed = 1;
	log_msg(LOG_DEBUG, "Server ready to receive next bit");
}

void	wait_for_server_ack(void)
{
	t_server_state	*server;
	int				timeout_count;
	int				max_timeout;
	pid_t			my_pid;

	server = get_server_instance();
	my_pid = getpid();
	timeout_count = 0;
	max_timeout = 50000;
	log_msg(LOG_DEBUG, "Waiting for server acknowledgment...");
	while (!server->ready_to_proceed)
	{
		usleep(100);
		timeout_count++;
		if (timeout_count % 50000 == 0)
			check_transmission_ownership(my_pid, 0, 0);
		handle_timeout(timeout_count, max_timeout);
	}
	server->ready_to_proceed = 0;
	log_msg(LOG_DEBUG, "Server acknowledgment received");
}

static void	handle_transmission_wait_loop(t_server_state *server, pid_t my_pid,
		int *wait_count)
{
	while (server->transmission_active && !is_transmission_owner(my_pid))
	{
		usleep(10000);
		(*wait_count)++;
		if (*wait_count % 3000 == 0)
		{
			log_msg(LOG_INFO, "Still waiting for transmission slot");
			if (kill(server->pid, 0) == -1)
			{
				ft_printf("Error: Server process no longer exists\n");
				exit(EXIT_FAILURE);
			}
		}
		if (*wait_count > 60000)
		{
			ft_printf("Error: Transmission slot timeout\n");
			exit(EXIT_FAILURE);
		}
	}
}

void	wait_for_transmission_slot(t_client *data)
{
	t_server_state	*server;
	int				wait_count;
	pid_t			my_pid;

	(void)data;
	server = get_server_instance();
	my_pid = getpid();
	wait_count = 0;
	log_msg(LOG_INFO, "Waiting for transmission slot...");
	if (!server->transmission_active || is_transmission_owner(my_pid))
	{
		set_transmission_active(my_pid);
		log_msg(LOG_SUCCESS, "Transmission slot acquired");
		return ;
	}
	handle_transmission_wait_loop(server, my_pid, &wait_count);
	set_transmission_active(my_pid);
	log_msg(LOG_SUCCESS, "Transmission slot acquired");
}
