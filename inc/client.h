/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:19:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 18:34:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# include "utils.h"
# include <signal.h>
# include <sys/types.h>
# include <stdbool.h>
# include <stddef.h>

/* ========== CONFIGURATION CONSTANTS ========== */

/* Handshake configuration */
# define MAX_ATTEMPTS 10
# define DEFAULT_TIMEOUT_MS 2000
# define DEFAULT_POLL_INTERVAL_US 50000
# define DEFAULT_RETRY_DELAY_MS 100

/* Message constraints */
# define MAX_MESSAGE_LENGTH 4096
# define MIN_MESSAGE_LENGTH 1

/* Signal definitions */
# define DEFAULT_READY_SIGNAL SIGUSR1
# define DEFAULT_BUSY_SIGNAL SIGUSR2
# define DEFAULT_PING_SIGNAL SIGTERM

/* Status definitions */
# define AVAILABLE 1
# define BUSY 0
# define ERROR -1

/* ========== TYPE DEFINITIONS ========== */

/* Handshake result codes */
typedef enum e_handshake_result
{
	HS_SUCCESS = 1,
	HS_TIMEOUT = 0,
	HS_ERROR = -1,
	HS_INVALID_PARAMS = -2,
	HS_SERVER_DEAD = -3
}	t_handshake_result;

/* Validation error codes */
typedef enum e_validation_error
{
	VALID_SUCCESS = 0,
	VALID_INVALID_PID = -1,
	VALID_PID_UNREACHABLE = -2,
	VALID_INVALID_MESSAGE = -3,
	VALID_INVALID_ARGS = -4,
	VALID_MEMORY_ERROR = -5
}	t_validation_error;

/* Server state enumeration */
typedef enum e_server_state
{
	SERVER_READY,
	SERVER_BUSY,
	SERVER_UNKNOWN
}	t_server_state;

/* Client operation mode */
typedef enum e_client_mode
{
	MODE_SEND_MESSAGE,
	MODE_PING_ONLY,
	MODE_STATUS_CHECK
}	t_client_mode;

/* ========== CONFIGURATION STRUCTURES ========== */

/* Handshake configuration */
typedef struct s_handshake_config
{
	int	timeout_ms;
	int	poll_interval_us;
	int	max_retries;
	int	retry_delay_ms;
}	t_handshake_config;

/* Client configuration */
typedef struct s_client_config
{
	t_client_mode			mode;
	bool					verbose;
	bool					use_ping;
	t_handshake_config		handshake;
}	t_client_config;

/* ========== MAIN DATA STRUCTURES ========== */

/* Client structure */
typedef struct s_client
{
	pid_t					pid_client;
	pid_t					pid_server;
	const char				*msg;
	size_t					msg_len;
	t_client_config			config;
	int						exit_code;
}	t_client;

/* Handshake structure */
typedef struct s_handshake
{
	pid_t					server_pid;
	volatile sig_atomic_t	is_ready;
	int						ready_signal;
	int						busy_signal;
	int						ping_signal;
	t_handshake_config		config;
}	t_handshake;

/* ========== FUNCTION PROTOTYPES ========== */

/* === Handshake Management === */

/* Core handshake functions */
int		handshake_init(pid_t pid, int ready_sig, int busy_sig, int ping_sig);
void	handshake_cleanup(void);
void	handshake_reset(void);
int		handshake_is_ready(void);

/* Handshake operations */
int		handshake_wait(int timeout_ms, int poll_interval_us);
int		handshake_ping(void);
int		handshake_retry(int max_retries, int timeout_ms, int poll_us);
int		handshake_retry_with_ping(int max_retries, int timeout_ms, int poll_us);

/* Configuration */
int		handshake_set_config(const t_handshake_config *config);
int		handshake_get_config(t_handshake_config *config);

/* === Client Validation === */

/* PID validation */
int		process_pid(t_client *client, const char *pid_str);
int		is_process_alive(pid_t pid);

/* Message validation */
int		process_message(const char *msg);

/* Overall validation */
int		validate_client_setup(int argc, char **argv, t_client *client);

/* === Utility Functions === */

/* Error handling */
const char	*get_validation_error_string(int error_code);
const char	*get_handshake_error_string(int error_code);
int			is_validation_success(int result);
int			is_handshake_success(int result);

/* Configuration helpers */
void	init_default_client_config(t_client_config *config);
void	init_default_handshake_config(t_handshake_config *config);

/* === Message Processing === */

/* Message transmission */
int		send_message_to_server(const t_client *client);
int		send_bit_by_bit(pid_t server_pid, const char *message);

/* Message utilities */
size_t	calculate_transmission_time(const char *message);
int		prepare_message_for_transmission(const char *input, char **output);

/* === Client Operations === */

/* Main client workflow */
int		client_main(int argc, char **argv);
int		execute_client_operation(const t_client *client);

/* Client setup and cleanup */
int		setup_client(t_client *client, int argc, char **argv);
void	cleanup_client(t_client *client);

/* === Signal Handling === */

/* Signal utilities */
void	setup_signal_handlers(void);
void	cleanup_signal_handlers(void);
int		send_signal_safe(pid_t pid, int signal);

/* === Logging and Debug === */

/* Debug functions */
void	print_client_info(const t_client *client);
void	print_handshake_status(void);
void	print_config_info(const t_client_config *config);

/* Performance monitoring */
void	start_timer(void);
long	get_elapsed_time_ms(void);

/* ========== INLINE UTILITY FUNCTIONS ========== */

/* Quick status checks */
static inline int	client_has_valid_pid(const t_client *client)
{
	return (client && client->pid_server > 0 && client->pid_client > 0);
}

static inline int	client_has_valid_message(const t_client *client)
{
	return (client && client->msg && client->msg_len > 0);
}

static inline int	client_is_ready(const t_client *client)
{
	return (client_has_valid_pid(client) && client_has_valid_message(client));
}

/* Configuration shortcuts */
static inline void	enable_verbose_mode(t_client *client)
{
	if (client)
		client->config.verbose = true;
}

static inline void	enable_ping_mode(t_client *client)
{
	if (client)
		client->config.use_ping = true;
}

/* ========== EXTERNAL DEPENDENCIES ========== */

/* Functions that should be implemented elsewhere */
extern int	log_msg(int level, const char *message);
extern int	ft_printf(const char *format, ...);
extern void	*ft_memset(void *s, int c, size_t n);
extern int	ft_atoi(const char *str);
extern size_t	ft_strlen(const char *s);
extern int	ft_isdigit(int c);
extern int	ft_isprint(int c);

#endif /* CLIENT_H */