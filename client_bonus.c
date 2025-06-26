/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:46:31 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 14:15:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_bonus.h"

void		send_character_bits(t_bclient *client, unsigned char c);
static int	setup_signal_handlers(void);
static void	handle_signal(int signal, siginfo_t *info, void *context);
static void	send_message(t_bclient *client);

/**
 * Main function - Entry point for the bonus client
 * Handles initialization, signal setup, and message transmission
 */
int	main(int argc, char **argv)
{
	t_bclient	client;

	if (!initialize_client(&client, argc, argv))
		return (ft_printf("Usage: %s <server_pid> <message>\n", argv[0]), 1);
	get_client_instance(&client);
	if (!setup_signal_handlers())
		return (ft_printf("Error: Signal handler setup failed\n"), 1);
	send_message(&client);
	return (0);
}

/**
 * Setup signal handlers for SIGUSR1 and SIGUSR2
 */
static int	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handle_signal;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (0);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (0);
	return (1);
}

/**
 * Signal handler for SIGUSR1 (acknowledgment)
 */
static void	handle_signal(int signal, siginfo_t *info, void *context)
{
	t_bclient	*client;

	(void)info;
	(void)context;
	client = get_client_instance(NULL);
	if (signal == SIGUSR1)
		client->ack = 1;
}

/**
 * Send complete message bit by bit with handshake
 */
static void	send_message(t_bclient *client)
{
	const char	*msg;
	int			i;

	msg = client->legacy_client.message;
	i = 0;
	while (msg[i])
	{
		send_character_bits(client, (unsigned char)msg[i]);
		i++;
	}
	send_character_bits(client, 0);
}
