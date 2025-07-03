/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:15:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 15:24:24 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void	handle_acknowledgment(t_server_state *server)
{
	server->ready_to_proceed = 1;
	log_msg(LOG_DEBUG, "Server ready to receive next bit");
}

static void	handle_completion(void)
{
	ft_printf("Message was received by the server.\n");
	log_msg(LOG_SUCCESS, "Message delivery confirmed by server");
	exit(EXIT_SUCCESS);
}

static int	validate_signal_source(t_server_state *server, pid_t sender_pid)
{
	if (server->pid != 0 && sender_pid != server->pid)
	{
		log_msg(LOG_WARNING, "Ignoring signal from unexpected PID: %d",
			sender_pid);
		return (0);
	}
	return (1);
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	t_server_state	*server;

	(void)context;
	server = get_server_instance();
	log_msg(LOG_DEBUG, "Received signal %d from PID %d", signum, info->si_pid);
	if (!validate_signal_source(server, info->si_pid))
		return ;
	if (signum == SIGUSR2)
		handle_acknowledgment(server);
	else if (signum == SIGUSR1)
		handle_completion();
}

static void	validate_and_init(int argc, char **argv, t_client *data)
{
	t_parser_result	parse_result;

	parse_result = parse_arguments(argc, argv);
	if (parse_result != PARSER_SUCCESS)
		print_error_and_exit(parse_result);
	init_data(argv, data);
}

static void	establish_connection(t_client *data)
{
	if (!connect_to_server(data))
	{
		ft_printf("Failed to connect to server\n");
		exit(EXIT_FAILURE);
	}
}

static void	prepare_transmission(t_client *data, int msg_len)
{
	struct sigaction	sa;
	sigset_t			sigset;

	ft_printf("Sending message (%d characters)\n", msg_len);
	log_msg(LOG_INFO, "Message length: %d characters", msg_len);
	setup_signal_handlers(&sigset, &sa);
	start_transmission(data, msg_len);
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

void	start_transmission(t_client *data, int msg_len)
{
	wait_for_transmission_slot(data);
	ft_printf("Starting transmission (%d characters)...\n", msg_len);
	log_msg(LOG_INFO, "Starting header transmission (message length)");
	send_signals(&msg_len, 32, data);
	log_msg(LOG_INFO, "Header transmission complete, starting message content");
	send_message(data->msg, data);
	log_msg(LOG_INFO, "All data transmitted, waiting for final confirmation");
}

int	connect_to_server(t_client *data)
{
	log_msg(LOG_INFO, "Attempting to connect to server PID %d",
		data->server_pid);
	if (kill(data->server_pid, 0) == -1)
	{
		ft_printf("Error: Server process PID %d not accessible\n",
			data->server_pid);
		log_msg(LOG_ERROR, "Server process validation failed");
		return (0);
	}
	if (ping(data->server_pid) == 0)
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_client	data;
	int			msg_len;

	validate_and_init(argc, argv, &data);
	establish_connection(&data);
	msg_len = ft_strlen(argv[2]);
	prepare_transmission(&data, msg_len);
	while (1)
		pause();
	return (EXIT_SUCCESS);
}