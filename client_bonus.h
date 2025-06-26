/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:48:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 12:14:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_BONUS_H
# define CLIENT_BONUS_H
# include "client.h"

typedef t_client	t_legacy;

typedef struct s_bclient
{
	t_legacy		legacy_client;
	int				batch_size;
	int				ack_received;
	int				timeout;
	volatile int	ack;
}				t_bclient;

void *ft_realloc(void *ptr, size_t old_size, size_t new_size);

#endif