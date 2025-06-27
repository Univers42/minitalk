/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:48:24 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 12:59:58 by dlesieur         ###   ########.fr       */
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
	t_atomic_flag	ack;
}				t_bclient;

t_bclient	*get_client_instance(t_bclient *set);
int			initialize_client(t_bclient *client, int argc, char **argv);
void			send_character_bits(t_bclient *client, unsigned char c);
void			send_message_bonus(t_bclient *client);
#endif