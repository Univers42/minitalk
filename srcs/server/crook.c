/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crook.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:26:43 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 18:11:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	check_client_disconnection(t_client_state *client)
{
	if (client->actual_pid == 0)
	{
		log_msg(LOG_DEBUG, "Client disconnected normally");
		return (1);
	}
	return (0);
}
