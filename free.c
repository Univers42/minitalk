/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:42:19 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 13:42:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_bonus.h"
#include "client_bonus.h"

void	clenaup_still_reachable(int sig)
{
	t_server	*srv;

	srv = get_server();
	if (srv->msg_buf)
		free(srv->msg_buf);
	exit(0);
}
