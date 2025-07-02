/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 16:06:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 16:35:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "signal.h"

static void	ping(int signum)
{
	t_client *client;

	client = get_client(NULL);
	if (signum == SIGUSR2)
			client->ok = 1;
	else if(signum == SIGUSR1)
		ft_printf("message received\n");
}

/**
sigset is  a shield to prevent racing condition
it tracks the signals down in our case SIGUSR1 
and SIGUSR2, If we don't do that, the stateg gets
messed up.
now we use sigset, it won't interrupt the code, 
it's like making our implementatin in `do not disturb`mode

 */
void set_signal(t_sigaction *sa, sigset_t *set)
{
	sigemptyset(set);
	sigaddset(set, SIGUSR1);
	sigaddset(set, SIGUSR2);

	ft_memset(sa, 0, sizeof(t_sigaction));
	sa->sa_handler = ping;
	sa->sa_flags = SA_RESTART;
	sa->sa_mask = *set;

	sigaction(SIGUSR1, sa, NULL);
	sigaction(SIGUSR2, sa, NULL);
}

