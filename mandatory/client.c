/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 20:49:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

//forward declarations;
void		handle_signal(int signal, siginfo_t *info, void *context);
static void	send_char(t_client *client, char c);
static void	send_message(t_client *client);
int			init_client(t_client *client, int args, t_strings argv);

int main(void)
{
	t_sigaction	sa;
	t_client	client;

	if (init_client(&client, argc, argv) != 0)
	{
		log_msg(LOG_ERROR, "the initialization has failed");
		return  (1);
	}
	get_instance(&client);
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up signal handler"), 1);
	log_msg(LOG_INFO, "Client started. Sending message to PID %d",
		client.server_pid);
	send_message(&client);
	log_msg(LOG_WARNING, "doubt that we verify that"
		"the message was really sent or not\n");
	log_msg(LOG_SUCCESS, "Message sent successfully");
	return (0);
}

void	handle_signal(int signal, siginfo_t *info, void *context)
{
	t_client *client;

	((void)info, (void)context);
	client = get_instance(NULL);
	if (signal == SIGUSR1)
		client->ack = HAND_SHAKED;
}

static void	send_char(t_client *client, char c)
{
	int	bit;

	bit = BYTE_SIZE;
	while (bit--)
		send_bit(client, c, bit);
}

void send_message(t_client *client)
{
	t_string	str;

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
		return (log_msg(LOG_ERROR, "Invalid server PID: %s",argv[1]), 1);
	client->message = argv[2];
	return (0);
}