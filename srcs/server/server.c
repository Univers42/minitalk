#include "server.h"

t_client_state	*get_client_instance(void)
{
	static t_client_state	instance;
	static int				initialized = 0;

	if (!initialized)
	{
		ft_memset(&instance, 0, sizeof(t_client_state));
		instance.getting_header = 1;
		instance.msg.size_message = 0;
		initialized = 1;
	}
	return (&instance);
}

void	reset_client_state(t_client_state *client)
{
	if (client->msg.message)
	{
		free(client->msg.message);
		client->msg.message = NULL;
	}
	ft_bzero(client, sizeof(t_client_state));
	client->getting_header = 1;
	client->msg.size_message = 0;
}

static int	get_bit_value(int signum)
{
	if (signum == SIGUSR1)
		return (0);
	return (1);
}

void	memory_reserve_to_store_signals(void)
{
	t_client_state	*client;

	client = get_client_instance();
	ft_printf("SIZE_MSG: [%d]\n", client->msg.size_message);
	log_msg(LOG_INFO, "Allocating memory for message of %d bytes", client->msg.size_message);
	client->msg.message = malloc((client->msg.size_message + 1) * 1);
	if (client->msg.message == NULL)
	{
		log_msg(LOG_ERROR, "Memory allocation failed for %d bytes", client->msg.size_message);
		write(2, "Error: Malloc failed\n", 21);
		exit(EXIT_FAILURE);
	}
	client->getting_header = 0;
	client->getting_msg = 1;
	client->sig_count = 0;
	log_msg(LOG_SUCCESS, "Memory allocated, switching to message reception mode");
}

void	handle_header(int signum)
{
	const int		bit_value = get_bit_value(signum);
	t_client_state	*client;

	client = get_client_instance();
	if (client->sig_count == 0)
	{
		client->msg.size_message = 0;
		log_msg(LOG_DEBUG, "Starting header reception");
	}
	if (client->sig_count < HEADER_SIZE)
	{
		client->msg.size_message |= \
			(bit_value << (HEADER_SIZE - 1 - client->sig_count));
		client->sig_count++;
		log_msg(LOG_DEBUG, "Header bit %d/%d: %d (current size: %d)", 
			client->sig_count, HEADER_SIZE, bit_value, client->msg.size_message);
	}
	if (client->sig_count == HEADER_SIZE)
	{
		log_msg(LOG_INFO, "Header complete: message size = %d bytes", client->msg.size_message);
		memory_reserve_to_store_signals();
	}
}

static void	print_msg(void)
{
	t_client_state	*client;

	client = get_client_instance();
	write(1, HEADER_MESSAGE, 18);
	write(1, client->msg.message, client->msg.size_message);
	write(1, "\n", 1);
}

void	handle_msg(int signum)
{
	const int		bit_value = get_bit_value(signum);
	t_client_state	*client;
	int				bit_pos;
	pid_t			client_pid;

	client = get_client_instance();
	bit_pos = client->sig_count % 8;
	if (bit_pos == 0)
	{
		client->char_value = 0;
		log_msg(LOG_DEBUG, "Starting new character %d/%d", 
			client->msg_pos + 1, client->msg.size_message);
	}
	client->char_value |= (bit_value << (7 - bit_pos));
	client->sig_count++;
	log_msg(LOG_DEBUG, "Bit %d/8: %d (char value: %d)", 
		bit_pos + 1, bit_value, client->char_value);
	if (client->sig_count % 8 == 0)
	{
		client->msg.message[client->msg_pos] = client->char_value;
		log_msg(LOG_DEBUG, "Character complete: '%c' (ASCII: %d)", 
			client->char_value, client->char_value);
		client->msg_pos++;
		if (client->msg_pos >= client->msg.size_message)
		{
			client->msg.message[client->msg_pos] = '\0';
			log_msg(LOG_SUCCESS, "Message reception complete: %d characters received", 
				client->msg_pos);
			print_msg();
			client_pid = client->client_pid;
			if (client->msg.message)
			{
				free(client->msg.message);
				client->msg.message = NULL;
			}
			reset_client_state(client);
			log_msg(LOG_INFO, "Client state reset, ready for new connections");
			if (client_pid > 0)
			{
				log_msg(LOG_INFO, "Sending completion signal to client %d", client_pid);
				kill(client_pid, SIGUSR1);
			}
		}
	}
}