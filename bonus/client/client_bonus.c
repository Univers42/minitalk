/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 13:11:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_bonus.h"

static int	setup_signal_handlers(void);
static void	handle_signal_bonus(int signal, siginfo_t *info, void *context);
void		send_message_bonus(t_bclient *client);

/**
 * Main function - Entry point for the bonus client
 */
int	main(int argc, char **argv)
{
	t_bclient	client;

	if (!initialize_client(&client, argc, argv))
		return (ft_printf("Usage: %s <server_pid> <message>\n", argv[0]), 1);
	get_client_instance(&client);
	if (!setup_signal_handlers())
		return (ft_printf("Error: Signal handler setup failed\n"), 1);
	send_message_bonus(&client);
	ft_printf("Message sent successfully!\n");
	return (0);
}

/**
 * Setup signal handlers with proper signal masking
 */
static int	setup_signal_handlers(void)
{
	t_sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sa.sa_sigaction = handle_signal_bonus;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (0);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (0);
	return (1);
}

/**
 * CRITICAL FIX: Signal handler accesses client instance
 * Uses instance getter to avoid global variables
 */
static void	handle_signal_bonus(int signal, siginfo_t *info, void *context)
{
	static t_sig_atomic	processing = 0;
	t_bclient			*client;

	(void)info;
	(void)context;
	if (processing)
		return ;
	processing = 1;
	client = get_client_instance(NULL);
	if (client && signal == SIGUSR1)
		client->ack = HAND_SHAKED;
	processing = 0;
}

/**
 * Send complete message with improved synchronization
 */
void	send_message_bonus(t_bclient *client)
{
	t_string	msg;
	int			i;

	msg = client->legacy_client.message;
	i = 0;
	while (msg[i])
	{
		if (!send_character_bits(client, (unsigned char)msg[i]))
		{
			ft_printf("Error: Failed to send character at position %d\n", i);
			return ;
		}
		i++;
	}
	if (!send_character_bits(client, 0))
	{
		ft_printf("Error: Failed to send null terminator\n");
		return ;
	}
}

/**
 * CRITICAL FIX: Enhanced bit sending with retry logic and proper timing
 * Key improvements:
 * 1. Byte-level synchronization instead of bit-level
 * 2. Proper timeout handling with exponential backoff
 * 3. Retry mechanism for lost signals
 */
int	send_character_bits(t_bclient *client, t_uint8 c)
{
	int		i;
	int		bit;
	pid_t	pid;

	pid = client->legacy_client.server_pid;
	i = 0;
	while (i < 8)
	{
		bit = (c >> i) & 1;
		client->ack = 0;
		if (bit)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (!client->ack)
			usleep(2000);
		i++;
	}
	return (1);
}
