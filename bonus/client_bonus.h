/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:48 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 21:13:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_BONUS_H
# define CLIENT_BONUS_H

# include "../mandatory/client.h"

typedef t_client	t_legacy;

typedef struct s_bclient
{
	t_legacy		legacy_client;
	int				batch_size;
	t_flag			ack_received;
	int				timeout;
	t_sig_atomic	ack; /* Made atomic for signal safety */
}					t_bclient;

/* Function prototypes */
t_bclient	*get_client_instance(t_bclient *set);
int			initialize_client(t_bclient *client, int argc, char **argv);
int			send_character_bits(t_bclient *client, t_uint8 c);
void		send_message_bonus(t_bclient *client);

#endif