/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 10:46:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft/libft.h"
# include <sys/types.h>
# include <signal.h>
# include "../minitalk.h"
# define BUFFER_SIZE 1024

typedef struct s_client_state
{
	pid_t					client_pid;
	t_uint8					byte;
	int						bit_index;
	int						byte_index;
	t_string				buffer;
	size_t					buffer_size;
	struct s_client_state	*next;
}							t_client_state;

t_client_state	**get_clients(t_client_state **set);
t_client_state	*get_client_state(pid_t client_pid);
void			handle_signal(int signal, siginfo_t *info, void *context);
void			free_all_clients(void);
int				expand_client_buffer(t_client_state *state);
char			get_printable_char(t_uint8 byte);
void			handle_message_completion(t_client_state *state,
					pid_t client_pid);
int				handle_buffer_expansion(t_client_state *state,
					pid_t client_pid);
void			reset_client_state(t_client_state *state);
#endif