/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:19:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/03 05:01:11 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "libft.h"
# include "auto_parser.h"

# define RETRY_TIMES 		20
# define RETRY_TIME 		1
# define RETRY_INTERVALS 	100
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
	int		transmission_active;
	pid_t	current_client_pid;
	int		transmission_id;
	int		last_sequence;
	int		ack_count;
}			t_server_state;

/* Singleton functions */
t_server_state	*get_server_instance(void);
void			reset_server_state(void);
void			set_transmission_active(pid_t client_pid);
int				is_transmission_owner(pid_t client_pid);
void			end_transmission(void);

/* Parser functions */
t_parser_result	parse_arguments(int argc, char **argv);
int				validate_pid_string(const char *str);

/* Communication functions */
int				ping(int pid);
void			send_signals(void *data, size_t bit_length, t_client *info);
void			send_message(char *str, t_client *data);
void			wait_for_transmission_slot(t_client *data);

/* Utility functions */
void			init_data(char **argv, t_client *data);
void			print_error_and_exit(t_parser_result result);
int				ft_printf(const char *format, ...);
void			handle_ping_response(int signum, t_server_state *server, pid_t pid);
void			log_ping_attempt(int attempt, int max_attempts);
void			log_ping_result(int attempt, int success);
void			setup_ping_signals(struct sigaction *sa, sigset_t *sigset);
void			ping_handler(int signum, siginfo_t *info, void *context);
int				calculate_checksum(const char *data, int length);
#endif
