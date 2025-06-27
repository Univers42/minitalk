/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:40:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 19:34:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_server	*get_srv_instance(t_server *set)
{
	static t_server	*srv = NULL;

	if (set)
		srv = set;
	return (srv);
}
