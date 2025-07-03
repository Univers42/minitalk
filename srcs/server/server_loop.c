/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 03:20:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 03:16:06 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

#define TIMEOUT 5
#define SLEEP_TIME 500000

void	clean_global(void)
{
	t_client_state	*client;

	client = get_client_instance();
	if (client->actual_pid > 0)
		log_msg(LOG_WARNING, "Cleaning up client state for PID %d",
			client->actual_pid);
	client->client_activity = 0;
	if (client->msg.message != NULL)
	{
		write(2, "Cleaning up incomplete message from client\n", 43);
		log_msg(LOG_ERROR, "Incomplete message cleanup: %d bytes",
			client->msg.size_message);
		free(client->msg.message);
		client->msg.message = NULL;
	}
	reset_client_state(client);
	log_msg(LOG_INFO, "Client state cleaned, ready for new connections");
}

void	keep_server_up(void)
{
	while (1)
		check_clean_status();
}
