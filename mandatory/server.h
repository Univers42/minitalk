/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 21:03:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H
# include "../minitalk.h"
# include <sys/types.h>
# define BUFFER_SIZE 1024

typedef struct s_server
{
	t_pid			client_pid;
	t_uint8			byte;
	int				byte_index;
	int				bit_index;
	t_string		buffer;
	size_t			buffer_size;
	t_flag			running;
	struct s_server	*next;
}					t_server;

t_server		**get_clients(t_server **set);
t_client_state	*get_client_state(t_pid client_pid);
void			handle_signal(int signal, siginfo_t *info, void *context);
void			free_all_clients(void);
int				expand_clients_buffer(t_server *state);
char			get_printable_char(t_uint8 byte);
void			handle_message_completion(t_server *state, t_pid client_pid);
int				handle_buffer_expansion(t_server *state, t_pid client_pid);
void			reset_client_state(t_server *state);
#endif