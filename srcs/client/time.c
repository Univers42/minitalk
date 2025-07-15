/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:22:44 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 03:56:25 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

/**
 * the main function that tries out to reach the client until it hits
 * the limit imposed
 */
int	handle_timeouts(int pid)
{
	int	i;
	int	result;

	i = 0;
	log_msg(LOG_INFO, "Starting ping sequence to server PID %d", pid);
	while (++i <= RETRY_TIMES)
	{
		result = attempt_ping(pid, i);
		if (result == 1)
			return (0);
		if (result == -1)
			return (1);
	}
	log_msg(LOG_ERROR, "Server did not respond after %d attempts", RETRY_TIMES);
	return (1);
}

/**
 * retry protocol just use to be sure that we don't break the sending protocol
 * too early
 */
void	handle_retry_delay(int attempt)
{
	if (attempt > RETRY_TIMES / 2)
	{
		log_msg(LOG_INFO, "Server appears busy, increasing retry delay");
		usleep(attempt * 500000);
	}
	else
		usleep(attempt * 100000);
}

/**
 * check if the timeout has been outpassed each time
 * we need to verify that to avoid an infite pending state
 */
int	handle_timeout(int timeout_count, int max_timeout)
{
	if (timeout_count > max_timeout)
	{
		ft_printf("Error: Server acknowledgment timeout\n");
		log_msg(LOG_ERROR, "Timeout waiting for server acknowledgment");
		exit(EXIT_FAILURE);
	}
	return (0);
}
