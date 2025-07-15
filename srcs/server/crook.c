/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crook.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:26:43 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 04:31:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

/**
 * debug info
 */
int	check_client_disconnection(t_client_state *client)
{
	if (client->actual_pid == 0)
	{
		log_msg(LOG_DEBUG, "Client disconnected normally");
		return (1);
	}
	return (0);
}

/**
 * not used legacy code
 */
int	calculate_checksum(const char *data, int length)
{
	int	checksum;
	int	i;

	checksum = 0;
	i = 0;
	while (i < length)
	{
		checksum ^= data[i];
		checksum = (checksum << 1) | (checksum >> 31);
		i++;
	}
	return (checksum);
}

/**
 * quick condition to know which bit was sent
 */
int	get_bit_value(int signum)
{
	if (signum == SIGUSR1)
		return (0);
	return (1);
}

/**
 * the memory reserver
 */
void	memory_reserve_to_store_signals(void)
{
	t_client_state	*client;

	client = get_client_instance();
	ft_printf("SIZE_MSG: [%d]\n", client->msg.size_message);
	log_msg(LOG_INFO, "Allocating memory for message of %d bytes",
		client->msg.size_message);
	client->msg.message = malloc((client->msg.size_message + 1) * 1);
	if (client->msg.message == NULL)
	{
		log_msg(LOG_ERROR, "Memory allocation failed for %d bytes",
			client->msg.size_message);
		write(2, "Error: Malloc failed\n", 21);
		exit(EXIT_FAILURE);
	}
	client->getting_header = 0;
	client->getting_msg = 1;
	client->sig_count = 0;
	log_msg(LOG_SUCCESS, "Memory allocated, "
		"switching to message reception mode");
}
