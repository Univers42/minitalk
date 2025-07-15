/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:32:10 by codespace         #+#    #+#             */
/*   Updated: 2025/07/15 04:16:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft.h"
# include "auto_parser.h"

# define HEADER_SIZE 32
# define HEADER_MESSAGE "Message received: "
# define SIGNAL_RECEIVED SIGUSR1
# define SERVER_READY SIGUSR1
# define SERVER_BUSY SIGUSR2
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define TIMEOUT	50000
# define SLEEP_TIME 50000

/**
 * @brief Structure to hold message data and metadata
 */
typedef struct s_message
{
	char	*message;		//pointer to dynamically allocated message content
	int		size_message;	//Expected tot length of the message in characters
}	t_message;

/**
 * @brief Structure to maintain client connection state and mesg reception
 */
typedef struct s_client_state
{
	pid_t		actual_pid;		//PID o fhte currently active/connected client
	pid_t		client_pid;		//PID of the client sending current signal
	int			getting_header;	//1 if server is receiving head data : 0
	int			getting_msg;	//1 if server is receiving msg content:	0
	int			sig_count;		// Total nbr signal received from curr client
	int			char_value;		//curr char being assembled from bits
	int			msg_pos;		//curr position mesg bfufer (character index)
	int			client_activity;//1 if client is actively sending data: 0
	int			transmission_active; // 1 if a transmission session	WIP
	int			queue_position;		//Pos of clien in the server's proc queue
	int			sequence_number;	// Current sequence number for packet order
	int			expected_checksum;	//expected checksum value for data verif
	int			calculated_checksum;// Calculated checksum value from data
	t_message	msg;				//Message struct containing the msg data
}	t_client_state;

/* Singleton functions */
t_client_state	*get_client_instance(void);
void			reset_client_state(t_client_state *client);

/* Queue management functions */
int				is_server_busy(void);
void			set_server_busy(pid_t client_pid);

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
int				get_bit_value(int signum);
void			process_character(t_client_state *client,
					int bit_value, int bit_pos);
void			print_message(t_client_state *client);
void			handle_complete_message(t_client_state *client);
int				check_client_disconnection(t_client_state *client);
int				check_client_activity(t_client_state *client, int *i);
void			handle_timeout(t_client_state *client, int i);
void			send_completion_signal(pid_t client_pid);

/* Checksum functions */
int				calculate_checksum(const char *data, int length);
void			send_multiple_acks(pid_t client_pid);
int				lost_signal(int s_si_pid, int signum, void *context);
void			send_completion_signal(pid_t client_pid);
void			log_character_completion(t_client_state *client);
int				monitor_client_timeout(t_client_state *client);
#endif
