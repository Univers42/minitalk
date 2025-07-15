/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:46:56 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 03:12:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * here are the time to speak about sigset and sigaction and their relations
 * in C, signals are managed using functions liek `sigaddset, sigemptyset,
 * sigfillset,...`to manipulate signal sets for controlling which signal a
 * process handlers, blocks or ignore. These are criticle for robust
 * server-client communication, ensuring signals like `SIGUSR1` and `SIGUSR2`
 * are used correctly without interference from other signals or 
 * race conditions
 * `sigaddset` : Add a specific signal to a signal set
 * `sigset_t` : Data structures that store a set of signals, used to define
 * which signal to `block`, `unblock` or `handle`
 * these functions allow precise control over signal handling, ensuring the
 * server and client respond only to intended signals for communication
 * while avoiding unexprected behavior from other signals (SIGINT or SIGTERM)
 * Selective Signal Handling: Ensure only specific signals 
 * (SIGUSR1 for client-to-server messages) are processed, ignoring others.
 
    Prevent signals from interrupting critical sections of code,
 * avoiding race conditions in server-client communication.

 * Robustness: Properly managing signal sets ensures predictable behavior,
 * especially in concurrent or multi-process environments.

 * Flexibility: Signal sets allow dynamic addition/removal of signals to handle,
 * supporting complex communication protocols.
 */

/**
 * initlize the members of the struct
 * recheck if server_pid has been correclyt initialized with a value
 * if not send error
 * otherwise OK
 */
void	init_data(char **argv, t_client *data)
{
	ft_memset(data, 0, sizeof(t_client));
	data->server_pid = ft_atoi(argv[1]);
	data->client_pid = getpid();
	data->msg = argv[2];
	if (data->server_pid <= 0)
	{
		ft_printf("Error: %s\n", BAD_SIGNAL);
		log_msg(LOG_ERROR, "Invalid server PID: %s", argv[1]);
		exit(EXIT_FAILURE);
	}
	log_msg(LOG_DEBUG, "Client PID: %d, Server PID: %d", data->client_pid,
		data->server_pid);
}

/**
 *
 */
void	setup_ping_signals(struct sigaction *sa, sigset_t *sigset)
{
	sigemptyset(sigset);
	sigaddset(sigset, SIGUSR1);
	sigaddset(sigset, SIGUSR2);
	sa->sa_flags = SA_SIGINFO;
	sa->sa_sigaction = ping_handler;
	sa->sa_mask = *sigset;
	sigaction(SIGUSR1, sa, NULL);
	sigaction(SIGUSR2, sa, NULL);
}

/**
 * Sigemptyset does the same job as the ft_memset
 * just for nomenclature it is best to use this function
 * as it is more over allowed by the school.
 * sa->sa_flags tells to use the three parameters of the signal pattern
 * from the struct of siginfo, this way we can use si_pid
 * sa->sa_sigaction = signal_handler assign the fn as the handler
 * for sending the bits
 * sa->sa_flags = restart and queuing system may be allowed
 * sigaction() register the configuration liking to the function
 * ensure SIGUSR1 OR SIGUSR2 when called triggers the
 * communication handler
 */
void	setup_signal_handlers(sigset_t *sigset, struct sigaction *sa)
{
	sigemptyset(sigset);
	sigaddset(sigset, SIGUSR1);
	sigaddset(sigset, SIGUSR2);
	sa->sa_flags = SA_SIGINFO;
	sa->sa_sigaction = signal_handler;
	sa->sa_mask = *sigset;
	sigaction(SIGUSR1, sa, NULL);
	sigaction(SIGUSR2, sa, NULL);
}

//simple initialization of all variables
void	reset_server_state(void)
{
	t_server_state	*server;

	server = get_server_instance();
	server->pid = 0;
	server->is_ready = 0;
	server->ready_to_proceed = 0;
	server->transmission_active = 0;
	server->current_client_pid = 0;
	server->transmission_id = 0;
	server->last_sequence = 0;
	server->ack_count = 0;
}

/**
 * This is the maine entry point of the sigaction and
 * sigset strucures
 * We use a pointer that point to its member memory to play
 * around with its datas
 * Once all is set we start the transmission between the server
 * and the client
 */
void	prepare_transmission(t_client *data, int msg_len)
{
	struct sigaction	sa;
	sigset_t			sigset;

	ft_printf("Sending message (%d characters)\n", msg_len);
	log_msg(LOG_INFO, "Message length: %d characters", msg_len);
	setup_signal_handlers(&sigset, &sa);
	start_transmission(data, msg_len);
}
