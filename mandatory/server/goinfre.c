/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   goinfre.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:53:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:58:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void clean_remaining_data(t_client_state *state)
{
	state->
	if (state->buffer != NULL)
	{
		log_msg(ERROR, "problem with the last client");
		free_all_clients(state->buffer);
	}
}

int	check_clean_status(void)
{
	
}

void	await(t_client_state *state)
{
	while (state->await)
		check_clean_status();
}