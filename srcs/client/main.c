/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:56:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 17:30:31 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client.h"

int main(int argc, char **argv)
{
	t_client	client;
	t_sigaction	sa;
	sigset_t	sigset;

	if (!hook(argc, argv, &client))
		return (log_msg(LOG_ERROR, "init didn't go well"), 1);
	set_signal(sa, sigset);
	icp();
}