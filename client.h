/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:43:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 12:57:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H
# include "libft.h"

# define BYTE_SIZE 8

typedef struct s_client
{
	pid_t		server_pid;
	char		*message;
	bool		await;
	pid_t		client_pid;
}				t_client;

#endif