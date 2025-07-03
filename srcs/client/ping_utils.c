/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 03:15:33 by codespace         #+#    #+#             */
/*   Updated: 2025/07/03 07:57:43 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

void	handle_ping_response(int signum, t_server_state *server, pid_t pid)
{
	if (signum == SIGUSR1)
	{
		// Server is ready and available
		server->is_ready = 1;
		log_msg(LOG_SUCCESS, "Server ready signal received from PID %d", pid);
	}
	else if (signum == SIGUSR2)
	{
		// Server is busy with another client - need to wait
		server->is_ready = 0;
		log_msg(LOG_WARNING, "Server busy signal received from PID %d - will retry", pid);
	}
}

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

void	send_message(char *str, t_client *data)
{
	int				i;
	pid_t			my_pid;
	int				total_chars;
	int				progress_interval;

	i = 0;
	my_pid = getpid();
	total_chars = ft_strlen(str);
	
	// Calculate progress reporting interval (every 5% or every 1000 chars, whichever is larger)
	progress_interval = (total_chars / 20); // 5%
	if (progress_interval < 1000)
		progress_interval = 1000;
	if (progress_interval > 10000)
		progress_interval = 10000;
	
	log_msg(LOG_INFO, "Starting message transmission: %d characters", total_chars);
	
	while (str[i])
	{
		// Verify we still own the transmission before each character
		if (!is_transmission_owner(my_pid))
		{
			log_msg(LOG_ERROR, "Lost transmission ownership during message send");
			exit(EXIT_FAILURE);
		}
		
		// Progress reporting for large messages
		if (total_chars > 5000 && (i % progress_interval == 0) && i > 0)
		{
			int percentage = (i * 100) / total_chars;
			ft_printf("Progress: %d%% (%d/%d characters)\n", percentage, i, total_chars);
		}
		
		log_msg(LOG_DEBUG, "Sending character %d: '%c' (ASCII: %d)",
			i + 1, str[i], str[i]);
		send_signals(&str[i], 8, data);
		i++;
	}
	
	if (total_chars > 5000)
		ft_printf("Progress: 100%% (%d/%d characters) - Complete!\n", i, total_chars);
	log_msg(LOG_SUCCESS, "Message transmission complete: %d characters sent", i);
}
