/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:15:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 08:02:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	t_server_state	*server;
	const char		*signal_name;
	pid_t			my_pid;

	(void)context;
	server = get_server_instance();
	my_pid = getpid();
	
	if (signum == SIGUSR1)
		signal_name = "SIGUSR1";
	else
		signal_name = "SIGUSR2";
	
	log_msg(LOG_DEBUG, "Received signal %s from PID %d", signal_name, info->si_pid);
	
	// Only accept signals from the server we're connected to
	if (server->pid != 0 && info->si_pid != server->pid)
	{
		log_msg(LOG_WARNING, "Ignoring signal from unexpected PID: %d", info->si_pid);
		return ;
	}
	
	// Only process signals if we own the transmission
	if (server->transmission_active && !is_transmission_owner(my_pid))
	{
		log_msg(LOG_DEBUG, "Ignoring signal - not transmission owner");
		return ;
	}
	
	if (signum == SIGUSR2)
	{
		// Server acknowledgment - always accept (may receive multiple)
		server->ready_to_proceed = 1;
		server->ack_count++;
		log_msg(LOG_DEBUG, "Server ready to receive next bit (ack #%d)", server->ack_count);
	}
	else if (signum == SIGUSR1)
	{
		ft_printf("Message was received by the server.\n");
		log_msg(LOG_SUCCESS, "Message delivery confirmed by server");
		end_transmission();
		exit(EXIT_SUCCESS);
	}
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
	int estimated_time_seconds;
	
	// Wait for our turn in the queue
	wait_for_transmission_slot(data);
	
	ft_printf("Starting transmission (%d characters)...\n", msg_len);
	log_msg(LOG_INFO, "About to send message length: %d (0x%x)", msg_len, msg_len);
	
	// Validate message length before sending
	if (msg_len <= 0 || msg_len > 10000000)
	{
		ft_printf("Error: Invalid message length: %d\n", msg_len);
		log_msg(LOG_ERROR, "Invalid message length: %d", msg_len);
		exit(EXIT_FAILURE);
	}
	
	// Calculate and display estimated transmission time
	estimated_time_seconds = (msg_len * 8 * 100) / 1000000; // 100μs per bit
	if (estimated_time_seconds > 60)
	{
		ft_printf("Large message detected (%d chars).\n", msg_len);
		ft_printf("Estimated transmission time: %d minutes %d seconds\n", 
			estimated_time_seconds / 60, estimated_time_seconds % 60);
		ft_printf("Please be patient...\n");
	}
	else if (estimated_time_seconds > 10)
	{
		ft_printf("This transmission will take approximately %d seconds\n", estimated_time_seconds);
	}
	
	// Send header (message length only for now)
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
	
	// Final check before attempting connection
	if (kill(data->server_pid, 0) == -1)
	{
		ft_printf("Error: Server process PID %d does not exist or is not accessible\n", 
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
	t_client			data;
	int					msg_len;
	struct sigaction	sa;
	sigset_t			sigset;
	t_parser_result		parse_result;

	parse_result = parse_arguments(argc, argv);
	if (parse_result != PARSER_SUCCESS)
		print_error_and_exit(parse_result);
	init_data(argv, &data);
	if (!connect_to_server(&data))
		return (EXIT_FAILURE);
	msg_len = ft_strlen(argv[2]);
	ft_printf("Sending message (%d characters)\n", msg_len);
	log_msg(LOG_INFO, "Message length: %d characters", msg_len);
	
	// Setup signal handlers after ping is complete
	setup_signal_handlers(&sigset, &sa);
	start_transmission(&data, msg_len);
	
	// Wait indefinitely for server confirmation
	while (1)
		pause();
	return (EXIT_SUCCESS);
}