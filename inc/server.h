/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:19:40 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 19:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft.h"
# include "auto_parser.h"
# include <signal.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# define HEADER_SIZE 32
# define HEADER_MESSAGE "Message received: "
# define SIGNAL_RECEIVED SIGUSR1
# define SERVER_READY SIGUSR1
# define SERVER_BUSY SIGUSR2
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

typedef struct s_message
{
	char	*message;
	int		size_message;
}	t_message;

typedef struct s_client_state
{
	pid_t		actual_pid;
	pid_t		client_pid;
	int			getting_header;
	int			getting_msg;
	int			sig_count;
	int			char_value;
	int			msg_pos;
	int			client_activity;
	t_message	msg;
}	t_client_state;

/* Singleton functions */
t_client_state	*get_client_instance(void);
void			reset_client_state(t_client_state *client);

/* Signal handling functions */
void			signal_handler(int signum, siginfo_t *info, void *context);
int				pong(int pid);
void			handle_header(int signum);
void			handle_msg(int signum);
void			memory_reserve_to_store_signals(void);

/* Server loop functions */
void			keep_server_up(void);
int				check_clean_status(void);
void			clean_global(void);

/* Utility functions */
int				ft_printf(const char *format, ...);
void			*ft_memset(void *s, int c, size_t n);
void			ft_bzero(void *s, size_t n);

#endif

