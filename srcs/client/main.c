/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:15:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:12:35 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

static void	handle_completion(void)
{
	ft_printf("Message was received by the server.\n");
	log_msg(LOG_SUCCESS, "Message delivery confirmed by server");
	exit(EXIT_SUCCESS);
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
