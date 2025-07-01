/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:58:08 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:26:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void	handle_signal(int signal)
{
	t_client	*client;

	client = get_instance(NULL);
	
	if (signal == SIGUSR2)
		client->ready = 1;
	if (signal == SIGUSR1)
	{
		client->ack = SHAKE_HANDED;
		log_msg(LOG_SUCCESS, "\rMessage was received by the server.");
	}
}

// Use SA_RESTART BECAUSE I DON'T USE ALL THE parameters into the sig
void	set_signal(t_sigaction sa)
{
	sigemptyset(&sa.sa_mask);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	sa.sa_handler = handle_signal;
	sa.sa_flags = SA_RESTART;
	sa.sa_mask = sigset;
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Error setting up signal handler 1"), 1);
	log_msg(LOG_INFO, "Client started. Sending message to PID %d",
		client.server_pid);
	if (sigaction(SIGUSR2, &sa, NULL) == -1)
		return (log_msg(LOG_ERROR, "Errot setting up signal handler 2"), 1);
}

void	get_signal_info(int bit_value, t_array sig, t_strings sig_name)
{
	if (bit_value)
	{
		*sig = SIGUSR1;
		*sig_name = "SIGUSR1";
	}
	else
	{
		*sig = SIGUSR2;
		*sig_name = "SIGUSR2";
	}
}
