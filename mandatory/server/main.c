/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:50:37 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:50:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	main(void)
{
	t_client_state		*clients;
	t_sigaction			sa;

	clients = NULL;
	get_clients(&clients);
	ft_printf("Server started on PID %d\n", getpid());
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up SIGUSR1 handler"), 1);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up SIGUSR2 handler"), 1);
	log_msg(LOG_INFO, "Server ready, waiting for messages...");
	while (1)
		pause();
	return (free_all_clients(), 0);
}
