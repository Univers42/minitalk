/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:38 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:53:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <sys/types.h>
# include <signal.h>
# include "../minitalk.h"

# define BUFFER_SIZE	2048
# define TIMEOUT		5
# define SLEEP_TIME		5000000

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
void			free_all_clients(void);
int				expand_client_buffer(t_client_state *state);
char			get_printable_char(t_uint8 byte);
void			handle_message_completion(t_client_state *state,
					pid_t client_pid);
int				handle_buffer_expansion(t_client_state *state,
					pid_t client_pid);
void			reset_client_state(t_client_state *state);
#endif