/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 02:17:43 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 02:49:02 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

//singleton to persist data accross callings
t_server_state	*get_server_instance(void)
{
	static t_server_state	server = {0, 0, 0, 0, 0, 0, 0, 0};

	return (&server);
}
