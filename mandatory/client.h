/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 20:49:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "minitalk.h"

# define BYTE_SIZE	8
# define HAND_SHAKED 1
typedef struct	s_client
{
	t_pid	server_pid;
	t_string	message;
	bool		await;
	t_pid		client_pid;
	t_flag		ack;
}				t_client;

#endif