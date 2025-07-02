#include "client.h"

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	t_server_state	*server;
	const char		*signal_name;

	(void)context;
	server = get_server_instance();
	signal_name = (signum == SIGUSR1) ? "SIGUSR1" : "SIGUSR2";
	
	log_msg(LOG_DEBUG, "Received signal %s from PID %d", signal_name, info->si_pid);
	
	if (signum == SIGUSR2)
	{
		server->ready_to_proceed = 1;
		log_msg(LOG_DEBUG, "Server ready to receive next bit");
	}
	else if (signum == SIGUSR1)
	{
		ft_printf("Message was received by the server.\n");
		log_msg(LOG_SUCCESS, "Message delivery confirmed by server");
		exit(EXIT_SUCCESS);
	}
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
	
	log_msg(LOG_INFO, "Attempting to connect to server PID %d", data.server_pid);
	if (ping(data.server_pid) == 0)
		return (EXIT_FAILURE);
	
	msg_len = ft_strlen(argv[2]);
	ft_printf("Sending message (%d characters)\n", msg_len);
	log_msg(LOG_INFO, "Message length: %d characters", msg_len);
	
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	sa.sa_mask = sigset;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	
	log_msg(LOG_INFO, "Starting header transmission (message length)");
	send_signals(&msg_len, 32, &data);
	log_msg(LOG_INFO, "Header transmission complete, starting message content");
	send_message(data.msg, &data);
	log_msg(LOG_INFO, "All data transmitted, waiting for final confirmation");
	
	while (1)
		pause();
	return (EXIT_SUCCESS);
}