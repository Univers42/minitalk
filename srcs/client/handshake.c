/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handshake.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 16:19:47 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 03:41:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	handle_acknowledgment(t_server_state *server)
{
	server->ready_to_proceed = 1;
	log_msg(LOG_DEBUG, "Server ready to receive next bit");
}

/**
 * gets eserver instance and current process ID
 * continuously checks for server acknowledgment
 * implements progressive timeout mechanism
 * periodically verifies transmission ownership
 * Reset acknowledgment flag after receiving response
 * maximum 50000 iterations with 100 micro second dealys
 * each 100micro second the ownership is verified
 * use usleep(100) for efficient `polling`
 * calls handle_timeout when maximum timeout is reached
 * performas ownership check to prevent deadlocks
 * @param none
 * @note
 * This function is called after sending data to server
 * Essential for synchronuos communication
 * Prevents data corruption from premature transmissions
 */
void	wait_for_server_ack(void)
{
	t_server_state	*server;
	int				timeout_count;
	int				max_timeout;
	pid_t			my_pid;

	server = get_server_instance();
	my_pid = getpid();
	timeout_count = 0;
	max_timeout = 50000;
	log_msg(LOG_DEBUG, "Waiting for server acknowledgment...");
	while (!server->ready_to_proceed)
	{
		usleep(100);
		timeout_count++;
		if (timeout_count % 50000 == 0)
			check_transmission_ownership(my_pid, 0, 0);
		handle_timeout(timeout_count, max_timeout);
	}
	server->ready_to_proceed = 0;
	log_msg(LOG_DEBUG, "Server acknowledgment received");
}

/**
 * This fucntion is continously monitoring if transmission is active
 * and process doens't own it
 * this report every 3000 iterations (30 seconds)
 * Verify the server health check existence
 * exit with failute ater 10 minutes
 * @param server 
 * @param my_pid
 * @param wait_count
 * @note exit if server no longer exist or if waiting exceed limits
 * called interally by `wait_for_transmission_slot`
 */
static void	handle_transmission_wait_loop(t_server_state *server, pid_t my_pid,
		int *wait_count)
{
	while (server->transmission_active && !is_transmission_owner(my_pid))
	{
		usleep(10000);
		(*wait_count)++;
		if (*wait_count % 3000 == 0)
		{
			log_msg(LOG_INFO, "Still waiting for transmission slot");
			if (kill(server->pid, 0) == -1)
			{
				ft_printf("Error: Server process no longer exists\n");
				exit(EXIT_FAILURE);
			}
		}
		if (*wait_count > 60000)
		{
			ft_printf("Error: Transmission slot timeout\n");
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * purpose : acquire exclusive tranmission rights for the current
 * client process, ensuring only one client can transmit at a time
 * behavior :
 * fast path : if transmission is inactive or already owned, immediately
 * adquire slot
 * 2. slow path: if transmission is active by another process,
 * enter waiting loop
 * 3. slot acquisition: set transmission as active for curren tprocess
 * 4. report acquisition status
 */
void	wait_for_transmission_slot(t_client *data)
{
	t_server_state	*server;
	int				wait_count;
	pid_t			my_pid;

	(void)data;
	server = get_server_instance();
	my_pid = getpid();
	wait_count = 0;
	log_msg(LOG_INFO, "Waiting for transmission slot...");
	if (!server->transmission_active || is_transmission_owner(my_pid))
	{
		set_transmission_active(my_pid);
		log_msg(LOG_SUCCESS, "Transmission slot acquired");
		return ;
	}
	handle_transmission_wait_loop(server, my_pid, &wait_count);
	set_transmission_active(my_pid);
	log_msg(LOG_SUCCESS, "Transmission slot acquired");
}
