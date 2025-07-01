/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 14:34:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

//forward declaration
static void	handle_signal(int signal);
static void	send_byte(t_client *client, char c);
static void	send_message(t_client *client);
static int	init_client(t_client *client, int argc, t_strings argv);

int	main(int argc, char **argv)
{
	t_sigaction			sa;
	t_client			client;

	if (init_client(&client, argc, argv) != 0)
		return (1);
	get_instance(&client);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handle_signal;
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up signal handler"), 1);
	log_msg(LOG_INFO, "Client started. Sending message to PID %d",
		client.server_pid);
	send_message(&client);
	log_msg(LOG_SUCCESS, "Message sent successfully");
	return (0);
}

static void	handle_signal(int signal)
{
	t_client	*client;

	client = get_instance(NULL);
	if (signal == SIGUSR1)
		client->ack = SHAKE_HANDED;
}

static void	send_byte(t_client *client, char c)
{
	int	b_seq;

	b_seq = BYTE_SIZE;
	while (b_seq--)
		send_bit(client, c, b_seq);
}

// Sends the message, including the null terminator
static void	send_message(t_client *client)
{
	t_string str;

	str = client->message;
	while (*str)
		send_byte(client, *str++);
	send_byte(client, '\0');
}

static int init_client(t_client *client, int argc, t_strings argv)
{
    ft_memset(client, 0, sizeof(t_client));
    if (argc != 3)
        return (log_msg(LOG_INFO, "Usage: %s <server_pid> <message>",
                argv[0]), 1);
    if (!is_valid_pid(argv[1]))
        return (log_msg(LOG_ERROR, "the PID has to be integrals"), 1);
    client->server_pid = ft_atoi(argv[1]);
    if (!is_pid_alive(client->server_pid))
        return (log_msg(LOG_ERROR, "Process with PID %d does not exist or is not accessible.",
                    client->server_pid), 1);
    client->message = argv[2];
    return (0);
}
