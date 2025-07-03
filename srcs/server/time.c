/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:25:12 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 18:24:54 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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
