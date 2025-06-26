/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:45:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 12:42:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft.h"
# define BUFFER_SIZE		1024

typedef struct sigaction	t_sigaction;

typedef struct s_server
{
	pid_t	pid;
	char	*buffer;
	int		bit_index;
	int		byte_index;
	bool	await;
}			t_server;

t_server	*get_srv_instance(t_server *set);
#endif