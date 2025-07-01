/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:31:33 by dlesieur         ###   ########.fr       */
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
	sigset_t			sigset;

	if (!init_client(&client, argc, argv))
		return (1);
	get_instance(&client);
	set_signals(&sa, &sigset);
	send_messages(&client, SEQ_BYTE);
	log_msg(LOG_SUCCESS, "Message sent successfully");
	return (0);
}

/**
 */
static int init_client(t_client *client, int argc, t_strings argv)
{
    ft_memset(client, 0, sizeof(t_client));
	
    if (argc != 3)
        return (log_msg(LOG_INFO, "Usage: %s <server_pid> <message>",
                argv[0]), 0);
    if (!is_valid_pid(argv[1]))
        return (log_msg(LOG_ERROR, "the PID has to be integrals"), 1);
    client->server_pid = ft_atoi(argv[1]);
	client->client_pid = getpid();
    if (!is_pid_alive(client->server_pid))
        return (log_msg(LOG_ERROR, "Process with PID %d does not exist or is not accessible.",
                    client->server_pid), 0);
    client->message = argv[2];
    return (1);
}
