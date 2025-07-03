/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:27:13 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 17:10:38 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	log_ping_attempt(int attempt, int max_attempts)
{
	log_msg(LOG_DEBUG, "Sending ping signal %d/%d to server",
		attempt, max_attempts);
	log_msg(LOG_INFO, "Waiting for server response (attempt %d/%d)",
		attempt, max_attempts);
}

void	log_ping_result(int attempt, int success)
{
	if (success)
		log_msg(LOG_SUCCESS, "Server responded on attempt %d", attempt);
	else
		log_msg(LOG_WARNING, "No response on attempt %d, retrying...", attempt);
}

void	log_ping_signal(int signum, pid_t sender_pid)
{
	const char	*signal_name;

	if (signum == SIGUSR1)
		signal_name = "SIGUSR1 (READY)";
	else
		signal_name = "SIGUSR2 (BUSY)";
	log_msg(LOG_DEBUG, "Ping handler received %s from PID %d",
		signal_name, sender_pid);
}
