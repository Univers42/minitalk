#include "libft.h"
#include "client.h"
#include <unistd.h>
#include <sys/types.h>

/* Error codes for better debugging */
typedef enum e_validation_error
{
	VALID_SUCCESS = 0,
	VALID_INVALID_PID = -1,
	VALID_PID_UNREACHABLE = -2,
	VALID_INVALID_MESSAGE = -3,
	VALID_INVALID_ARGS = -4,
	VALID_MEMORY_ERROR = -5
}	t_validation_error;

/* Forward declarations */
static int	validate_pid_format(const char *pid_str);
static int	validate_pid_reachable(pid_t pid);
static int	validate_message_content(const char *msg);
static int	extract_and_validate_pid(t_client *client, const char *pid_str);

/* ========== PID VALIDATION ========== */

static int	validate_pid_format(const char *pid_str)
{
	int		i;
	pid_t	pid;

	if (!pid_str || !*pid_str)
		return (VALID_INVALID_PID);
	i = 0;
	if (pid_str[0] == '-' || pid_str[0] == '+')
		i = 1;
	while (pid_str[i])
	{
		if (!ft_isdigit(pid_str[i]))
			return (VALID_INVALID_PID);
		i++;
	}
	pid = ft_atoi(pid_str);
	if (pid <= 0 || pid > 2147483647)
		return (VALID_INVALID_PID);
	return (VALID_SUCCESS);
}

static int	validate_pid_reachable(pid_t pid)
{
	if (!is_process_alive(pid))
	{
		log_msg(LOG_ERROR, "Server PID is not reachable or doesn't exist");
		return (VALID_PID_UNREACHABLE);
	}
	return (VALID_SUCCESS);
}

static int	extract_and_validate_pid(t_client *client, const char *pid_str)
{
	int	validation_result;

	validation_result = validate_pid_format(pid_str);
	if (validation_result != VALID_SUCCESS)
	{
		log_msg(LOG_ERROR, "Invalid PID format");
		return (validation_result);
	}
	client->pid_server = ft_atoi(pid_str);
	client->pid_client = getpid();
	if (client->pid_client <= 0)
	{
		log_msg(LOG_ERROR, "Failed to get client PID");
		return (VALID_INVALID_PID);
	}
	validation_result = validate_pid_reachable(client->pid_server);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	log_msg(LOG_SUCCESS, "Server PID verified and reachable");
	return (VALID_SUCCESS);
}

/* ========== MESSAGE VALIDATION ========== */

static int	validate_message_content(const char *msg)
{
	int	i;

	if (!msg)
	{
		log_msg(LOG_ERROR, "Message is NULL");
		return (VALID_INVALID_MESSAGE);
	}
	if (!*msg)
	{
		log_msg(LOG_ERROR, "Message is empty");
		return (VALID_INVALID_MESSAGE);
	}
	i = 0;
	while (msg[i])
	{
		if (!ft_isprint(msg[i]) && msg[i] != '\n' && msg[i] != '\t')
		{
			log_msg(LOG_ERROR, "Message contains non-printable characters");
			return (VALID_INVALID_MESSAGE);
		}
		i++;
	}
	return (VALID_SUCCESS);
}

static int	validate_message_length(const char *msg)
{
	size_t	len;

	len = ft_strlen(msg);
	if (len == 0)
	{
		log_msg(LOG_ERROR, "Message is empty");
		return (VALID_INVALID_MESSAGE);
	}
	if (len > 4096)
	{
		log_msg(LOG_ERROR, "Message is too long (max 4096 characters)");
		return (VALID_INVALID_MESSAGE);
	}
	return (VALID_SUCCESS);
}

/* ========== ARGUMENT VALIDATION ========== */

static int	validate_argc(int argc, char **argv)
{
	if (argc != 3)
	{
		if (argv && argv[0])
			ft_printf("Usage: %s <server_pid> <message>\n", argv[0]);
		else
			ft_printf("Usage: ./client <server_pid> <message>\n");
		log_msg(LOG_ERROR, "Invalid number of arguments");
		return (VALID_INVALID_ARGS);
	}
	return (VALID_SUCCESS);
}

static int	validate_argv(char **argv)
{
	if (!argv || !argv[0] || !argv[1] || !argv[2])
	{
		log_msg(LOG_ERROR, "Invalid arguments provided");
		return (VALID_INVALID_ARGS);
	}
	return (VALID_SUCCESS);
}

/* ========== CLIENT INITIALIZATION ========== */

static int	initialize_client_struct(t_client *client)
{
	if (!client)
		return (VALID_MEMORY_ERROR);
	ft_memset(client, 0, sizeof(t_client));
	return (VALID_SUCCESS);
}

static int	setup_client_message(t_client *client, const char *message)
{
	int	validation_result;

	validation_result = validate_message_length(message);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	validation_result = validate_message_content(message);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	client->msg = message;
	client->msg_len = ft_strlen(message);
	log_msg(LOG_SUCCESS, "Message verified and valid");
	return (VALID_SUCCESS);
}

/* ========== PUBLIC API ========== */

int	process_pid(t_client *client, const char *pid_str)
{
	if (!client || !pid_str)
		return (VALID_INVALID_ARGS);
	return (extract_and_validate_pid(client, pid_str));
}

int	process_message(const char *msg)
{
	int	validation_result;

	validation_result = validate_message_length(msg);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	return (validate_message_content(msg));
}

int	validate_client_setup(int argc, char **argv, t_client *client)
{
	int	validation_result;

	validation_result = validate_argc(argc, argv);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	validation_result = validate_argv(argv);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	validation_result = initialize_client_struct(client);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	validation_result = process_pid(client, argv[1]);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	log_msg(LOG_SUCCESS, "PID processed successfully");
	validation_result = setup_client_message(client, argv[2]);
	if (validation_result != VALID_SUCCESS)
		return (validation_result);
	return (VALID_SUCCESS);
}

/* ========== UTILITY FUNCTIONS ========== */

const char	*get_validation_error_string(int error_code)
{
	static const char	*error_messages[] = {
		"Success",
		"Invalid PID format or value",
		"PID is not reachable",
		"Invalid message content",
		"Invalid arguments",
		"Memory allocation error"
	};

	if (error_code >= 0 || error_code < -5)
		return ("Unknown error");
	return (error_messages[-error_code]);
}

int	is_validation_success(int result)
{
	return (result == VALID_SUCCESS);
}