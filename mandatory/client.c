/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 10:44:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	handle_signal(int signal, siginfo_t *info, void *context)
{
	t_client	*client;

	(void)info;
	(void)context;
	client = get_instance(NULL);
	if (signal == SIGUSR1)
		client->ack = 1;
}

static void	send_char(t_client *client, char c)
{
	int	bit;

	bit = BYTE_SIZE;
	while (bit--)
		send_bit(client, c, bit);
}

// Sends the message, including the null terminator
void	send_message(t_client *client)
{
	char	*str;

	str = client->message;
	while (*str)
		send_char(client, *str++);
	send_char(client, '\0');
}

int	init_client(t_client *client, int argc, t_strings argv)
{
	ft_memset(client, 0, sizeof(t_client));
	if (argc != 3)
		return (log_msg(LOG_INFO, "Usage: %s <server_pid> <message>",
				argv[0]), 1);
	client->server_pid = ft_atoi(argv[1]);
	if (client->server_pid <= 0)
		return (log_msg(LOG_ERROR, "Invalid server PID: %s", argv[1]), 1);
	client->message = argv[2];
	return (0);
}

int	main(int argc, char **argv)
{
	t_sigaction			sa;
	t_client			client;

	if (init_client(&client, argc, argv) != 0)
		return (1);
	get_instance(&client);
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up signal handler"), 1);
	log_msg(LOG_INFO, "Client started. Sending message to PID %d",
		client.server_pid);
	send_message(&client);
	log_msg(LOG_SUCCESS, "Message sent successfully");
	return (0);
}