/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:27:58 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:32:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	check_clean_status(void)
{
	t_client_state	*client;
	static int		last_pid = 0;

	client = get_client_instance();
	if (client->actual_pid == 0)
	{
		last_pid = 0;
		sleep(1);
		return (1);
	}
	if (last_pid != client->actual_pid)
	{
		last_pid = client->actual_pid;
		log_msg(LOG_DEBUG, "Starting monitoring for client %d", last_pid);
	}
	return (monitor_client_timeout(client));
}

int	is_server_busy(void)
{
	t_client_state	*client;

	client = get_client_instance();
	return (client->transmission_active && client->actual_pid > 0);
}
