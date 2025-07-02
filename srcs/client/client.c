#include "client.h"

void	init_data(char **argv, t_client *data)
{
	ft_memset(data, 0, sizeof(t_client));
	data->server_pid = ft_atoi(argv[1]);
	data->client_pid = getpid();
	data->msg = argv[2];
	if (data->server_pid <= 0)
	{
		ft_printf("Error: %s\n", BAD_SIGNAL);
		log_msg(LOG_ERROR, "Invalid server PID: %s", argv[1]);
		exit(EXIT_FAILURE);
	}
	log_msg(LOG_DEBUG, "Client PID: %d, Server PID: %d", data->client_pid, data->server_pid);
}

void	send_signal(pid_t pid, int signal)
{
	const char	*signal_name = (signal == SIGUSR1) ? "SIGUSR1 (0)" : "SIGUSR2 (1)";
	
	if (kill(pid, signal) == -1)
	{
		ft_printf("Error: Signal sending failed\n");
		log_msg(LOG_ERROR, "Failed to send signal %d to PID %d", signal, pid);
		exit(EXIT_FAILURE);
	}
	log_msg(LOG_DEBUG, "Sent signal %s to server PID %d", signal_name, pid);
}

void	wait_for_server_ack(void)
{
	t_server_state	*server;
	int				timeout_count;

	server = get_server_instance();
	timeout_count = 0;
	log_msg(LOG_DEBUG, "Waiting for server acknowledgment...");
	while (!server->ready_to_proceed)
	{
		usleep(100);
		timeout_count++;
		if (timeout_count > 100000)
		{
			ft_printf("Error: Server acknowledgment timeout\n");
			log_msg(LOG_ERROR, "Timeout waiting for server acknowledgment after %d attempts", timeout_count);
			exit(EXIT_FAILURE);
		}
	}
	server->ready_to_proceed = 0;
	log_msg(LOG_DEBUG, "Server acknowledgment received after %d microseconds", timeout_count * 100);
}

void	send_signals(void *data, size_t bit_length, t_client *info)
{
	unsigned long long	value;
	int					i;
	int					bit_value;

	value = 0;
	if (bit_length == 8)
		value = *((unsigned char *)data);
	else if (bit_length == 32)
		value = *((unsigned int *)data);
	
	log_msg(LOG_INFO, "Sending %zu-bit value: %llu (0x%llx)", bit_length, value, value);
	
	i = bit_length - 1;
	while (i >= 0)
	{
		bit_value = (value & (1ULL << i)) ? 1 : 0;
		log_msg(LOG_DEBUG, "Sending bit %d/%zu: %d", (int)(bit_length - i), bit_length, bit_value);
		
		if (bit_value)
			send_signal(info->server_pid, CHAR_1);
		else
			send_signal(info->server_pid, CHAR_0);
		i--;
		wait_for_server_ack();
	}
	log_msg(LOG_SUCCESS, "Successfully sent %zu bits", bit_length);
}

void	send_message(char *str, t_client *data)
{
	int	i;

	i = 0;
	log_msg(LOG_INFO, "Starting message transmission: \"%s\"", str);
	while (str[i])
	{
		log_msg(LOG_DEBUG, "Sending character %d: '%c' (ASCII: %d)", i + 1, str[i], str[i]);
		send_signals(&str[i], 8, data);
		i++;
	}
	log_msg(LOG_SUCCESS, "Message transmission complete: %d characters sent", i);
}