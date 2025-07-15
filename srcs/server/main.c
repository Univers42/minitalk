/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/15 04:07:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

/**
 * @brief Handles new client connection attempts
 * 
 * This function checks if the server is available for a new client connection.
 * When actual_pid is 0, it means no client is currently connected, so the server
 * can accept the new connection.
 * 
 * Logic Flow:
 * - If no active client (actual_pid == 0): Accept new client and send acknowledgment
 * - If client already active: Reject the connection attempt
 * 
 * @param client Pointer to the client state structure
 * @param info Signal information containing sender's PID
 * @return 1 if new client was accepted, 0 if rejected
 */
static int	handle_new_client(t_client_state *client, siginfo_t *info)
{
	if (client->actual_pid == 0)
	{
		log_msg(LOG_INFO, "New client connection from PID %d", info->si_pid);
		client->client_pid = info->si_pid;
		return (pong(client->client_pid), 1);
	}
	return (0);
}

/**
 * @brief Handles signals from clients when server is busy with another client
 * 
 * This function implements a single-client server model by rejecting signals
 * from clients other than the currently active one. It sends a SERVER_BUSY
 * signal to inform the rejected client that the server is unavailable.
 * 
 * Logic Flow:
 * - Compare sender PID with current active client PID
 * - If different: Send busy signal and reject
 * - If same: Allow processing to continue
 * 
 * @param client Pointer to the client state structure
 * @param info Signal information containing sender's PID
 * @return 1 if signal was rejected (busy), 0 if allowed to proceed
 */
static int	handle_busy_server(t_client_state *client, siginfo_t *info)
{
	if (client->actual_pid != info->si_pid)
	{
		log_msg(LOG_WARNING, "Server busy, rejecting signal"
			" from PID %d (current client: %d)",
			info->si_pid, client->actual_pid);
		kill(info->si_pid, SERVER_BUSY);
		return (1);
	}
	return (0);
}

/**
 * @brief Processes data signals based on current server state
 * 
 * This function routes signal processing based on what type of data
 * the server is currently expecting to receive.
 * 
 * State Machine Logic:
 * - getting_msg state: Process message content bits
 * - getting_header state: Process header/metadata bits
 * - Always sends acknowledgment after processing
 * 
 * @param client Pointer to the client state structure
 * @param signum The signal number received (SIGUSR1 or SIGUSR2)
 */
static void	process_data_signal(t_client_state *client, int signum)
{
	if (client->getting_msg == 1)
		handle_msg(signum);
	else if (client->getting_header == 1)
		handle_header(signum);
	send_multiple_acks(client->client_pid);
}

/**
 * @brief Main signal handler for processing all incoming signals
 * 
 * This is the central signal processing function that coordinates
 * all signal handling logic. It implements a state machine that
 * handles different types of client interactions.
 * 
 * Processing Flow:
 * 1. Get client state instance
 * 2. Handle potential lost signals
 * 3. Ignore self-sent signals
 * 4. Try to accept new client connections
 * 5. Handle busy server scenarios
 * 6. Process signals from active client
 * 
 * @param signum The signal number (SIGUSR1 or SIGUSR2)
 * @param info Signal information structure containing sender details
 * @param context Signal context (unused but required by sigaction)
 */
void	signal_handler(int signum, siginfo_t *info, void *context)
{
	t_client_state	*client;

	client = get_client_instance();
	info->si_pid = lost_signal(info->si_pid, signum, context);
	if (info->si_pid == getpid())
		return ;
	if (handle_new_client(client, info))
		return ;
	if (handle_busy_server(client, info))
		return ;
	client->client_pid = info->si_pid;
	client->client_activity = 1;
	log_msg(LOG_DEBUG, "Processing signal %d from active client %d",
		signum, client->client_pid);
	process_data_signal(client, signum);
}

/**
 * @brief Processes data signals based on current server state
 * 
 * This function routes signal processing based on what type of data
 * the server is currently expecting to receive.
 * 
 * State Machine Logic:
 * - getting_msg state: Process message content bits
 * - getting_header state: Process header/metadata bits
 * - Always sends acknowledgment after processing
 * 
 * @param client Pointer to the client state structure
 * @param signum The signal number received (SIGUSR1 or SIGUSR2)
 */
int	main(void)
{
	struct sigaction	sa;
	sigset_t			sigset;
	pid_t				server_pid;

	server_pid = getpid();
	ft_printf("Server PID: %d\n", server_pid);
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sa.sa_mask = sigset;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	keep_server_up();
	return (0);
}
