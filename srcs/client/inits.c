/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:46:56 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:12:57 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

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

void	prepare_transmission(t_client *data, int msg_len)
{
	struct sigaction	sa;
	sigset_t			sigset;

	ft_printf("Sending message (%d characters)\n", msg_len);
	log_msg(LOG_INFO, "Message length: %d characters", msg_len);
	setup_signal_handlers(&sigset, &sa);
	start_transmission(data, msg_len);
}
