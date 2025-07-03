/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:19:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/03 01:00:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "libft.h"
# include "auto_parser.h"

# define RETRY_TIMES 		10
# define RETRY_TIME 		1
# define RETRY_INTERVALS 	20
# define SERVER_READY		SIGUSR1
# define SERVER_BUSY		SIGUSR2
# define CHAR_0 			SIGUSR1
# define CHAR_1 			SIGUSR2
# define USAGE				"Usage: ./client <server_pid> <message>"
# define BAD_SIGNAL			"Invalid server PID"

typedef enum e_parser_result
{
	PARSER_SUCCESS,
	PARSER_INVALID_ARGC,
	PARSER_INVALID_PID,
	PARSER_EMPTY_MESSAGE
}	t_parser_result;

typedef struct s_client
{
	pid_t	server_pid;
	pid_t	client_pid;
	char	*msg;
}			t_client;

typedef struct s_server_state
{
	pid_t	pid;
	int		is_ready;
	int		ready_to_proceed;
}			t_server_state;

/* Singleton functions */
t_server_state	*get_server_instance(void);
void			reset_server_state(void);

/* Parser functions */
t_parser_result	parse_arguments(int argc, char **argv);
int				validate_pid_string(const char *str);

/* Communication functions */
int				ping(int pid);
void			send_signals(void *data, size_t bit_length, t_client *info);
void			send_message(char *str, t_client *data);

/* Utility functions */
void			init_data(char **argv, t_client *data);
void			print_error_and_exit(t_parser_result result);
int				ft_printf(const char *format, ...);

#endif
