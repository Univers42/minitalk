/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:45:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 12:28:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft/libft.h"
# include <sys/types.h>
# include <signal.h>

# define BUFFER_SIZE 1024

typedef struct s_server
{
	pid_t	pid;
	char	*buffer;
	size_t	buffer_size;  // Added this field
	int		await;
}	t_server;

// Function prototypes
void		init_server(t_server *srv);
void		handle_signal(int signal, siginfo_t *info, void *context);
t_server	*get_srv_instance(t_server *srv);
#endif