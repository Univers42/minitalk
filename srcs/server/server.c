/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:22:54 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 03:06:00 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_client_state	*get_client_instance(void)
{
	static t_client_state	instance;
	static int				initialized = 0;

	if (!initialized)
	{
		ft_memset(&instance, 0, sizeof(t_client_state));
		instance.getting_header = 1;
		instance.msg.size_message = 0;
		initialized = 1;
	}
	return (&instance);
}

void	reset_client_state(t_client_state *client)
{
	if (client->msg.message)
	{
		free(client->msg.message);
		client->msg.message = NULL;
	}
	ft_bzero(client, sizeof(t_client_state));
	client->getting_header = 1;
	client->msg.size_message = 0;
}

int	get_bit_value(int signum)
{
	if (signum == SIGUSR1)
		return (0);
	return (1);
}

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

void	handle_header(int signum)
{
	const int		bit_value = get_bit_value(signum);
	t_client_state	*client;

	client = get_client_instance();
	if (client->sig_count == 0)
	{
		client->msg.size_message = 0;
		log_msg(LOG_DEBUG, "Starting header reception");
	}
	if (client->sig_count < HEADER_SIZE)
	{
		client->msg.size_message |= (bit_value
				<< (HEADER_SIZE - 1 - client->sig_count));
		client->sig_count++;
		log_msg(LOG_DEBUG, "Header bit %d/%d: %d (current size: %d)",
			client->sig_count, HEADER_SIZE, bit_value,
			client->msg.size_message);
	}
	if (client->sig_count == HEADER_SIZE)
	{
		log_msg(LOG_INFO, "Header complete: message size = %d bytes",
			client->msg.size_message);
		memory_reserve_to_store_signals();
	}
}
