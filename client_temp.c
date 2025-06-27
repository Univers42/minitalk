/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_temp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:45:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 19:30:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "log.h"

t_client *get_instance(t_client *set)
{
	static t_client *client = NULL;
	if (set)
		client = set;
	return (client);
}

void handle_signal(int signal, siginfo_t *info, void *context)
{
	t_client *client;

	(void)info;
	(void)context;
	client = get_instance(NULL);
	if (signal == SIGUSR1)
		client->ack = 1;
}

// Returns the value (0 or 1) of the bit at bit_index (7 to 0) in c, using math
static int get_bit_from_char(char c, int bit_index)
{
	int value;
	int divisor;

	value = (unsigned char)c;
	divisor = 1;
	for (int i = 0; i < bit_index; ++i)
		divisor *= 2;
	return (value / divisor) % 2;
}

// Sends a single character, bit by bit, using math
static void send_char(t_client *client, char c)
{
	for (int bit = 7; bit >= 0; --bit)
	{
		client->ack = 0;
		int bit_value = get_bit_from_char(c, bit);
		int sig = bit_value ? SIGUSR1 : SIGUSR2;
		if (kill(client->server_pid, sig) == -1)
		{
			log_msg(LOG_ERROR, "kill() failed for PID %d: %s", client->server_pid, strerror(errno));
			exit(1);
		}
		while (!client->ack)
			;
	}
}

// Sends the message, including the null terminator
void send_message(t_client *client)
{
	char *str = client->message;
	while (*str)
		send_char(client, *str++);
	send_char(client, '\0');
}

int init_client(t_client *client, int argc, char **argv)
{
	ft_memset(client, 0, sizeof(t_client));
	if (argc != 3)
		return (log_msg(LOG_INFO, "Usage: %s <server_pid> <message>", argv[0]), 1);
	client->server_pid = ft_atoi(argv[1]);
	if (client->server_pid <= 0)
		return (log_msg(LOG_ERROR, "Invalid server PID: %s", argv[1]), 1);
	client->message = argv[2];
	return (0);
}

int main(int argc, char **argv)
{
	t_client client;
	struct sigaction sa;

	if (init_client(&client, argc, argv) != 0)
		return (1);
	get_instance(&client);
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up signal handler"), 1);
	log_msg(LOG_INFO, "Client started. Sending message to PID %d", client.server_pid);
	send_message(&client);
	log_msg(LOG_SUCCESS, "Message sent successfully");
	return (0);
}