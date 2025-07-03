/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 03:30:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 03:13:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

#define TIMEOUT 5
#define SLEEP_TIME 500000

int	check_client_activity(t_client_state *client, int *i)
{
	if (client->client_activity)
	{
		client->client_activity = 0;
		*i = 0;
		return (1);
	}
	return (0);
}

int	check_client_disconnection(t_client_state *client)
{
	if (client->actual_pid == 0)
	{
		log_msg(LOG_DEBUG, "Client disconnected normally");
		return (1);
	}
	return (0);
}

void	handle_timeout(t_client_state *client, int i)
{
	if (i >= TIMEOUT && client->actual_pid > 0)
	{
		log_msg(LOG_WARNING, "Client timeout after %d seconds", TIMEOUT);
		clean_global();
	}
}

int	monitor_client_timeout(t_client_state *client)
{
	int	i;

	i = 0;
	while (i < TIMEOUT)
	{
		usleep(SLEEP_TIME);
		if (check_client_disconnection(client))
			return (0);
		if (check_client_activity(client, &i))
			continue ;
		i++;
	}
	handle_timeout(client, i);
	return (0);
}

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
