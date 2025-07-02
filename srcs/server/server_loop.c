#include "server.h"

#define TIMEOUT 5
#define SLEEP_TIME 500000

void	clean_global(void)
{
	t_client_state	*client;

	client = get_client_instance();
	if (client->actual_pid > 0)
		log_msg(LOG_WARNING, "Cleaning up client state for PID %d due to timeout", client->actual_pid);
	client->client_activity = 0;
	if (client->msg.message != NULL)
	{
		write(2, "Cleaning up incomplete message from client\n", 43);
		log_msg(LOG_ERROR, "Incomplete message cleanup: %d bytes allocated", client->msg.size_message);
		free(client->msg.message);
		client->msg.message = NULL;
	}
	reset_client_state(client);
	log_msg(LOG_INFO, "Client state cleaned, server ready for new connections");
}

int	check_clean_status(void)
{
	int				i;
	t_client_state	*client;
	static int		last_pid = 0;

	client = get_client_instance();
	if (client->actual_pid == 0)
	{
		last_pid = 0;
		return (sleep(1), 1);
	}
	if (last_pid != client->actual_pid)
	{
		last_pid = client->actual_pid;
		log_msg(LOG_DEBUG, "Starting timeout monitoring for client %d", last_pid);
	}
	i = 0;
	while (i < TIMEOUT)
	{
		usleep(SLEEP_TIME);
		if (client->actual_pid == 0)
		{
			log_msg(LOG_DEBUG, "Client disconnected normally");
			return (0);
		}
		if (client->client_activity)
		{
			client->client_activity = 0;
			i = 0;
		}
		else
			i++;
	}
	if (i >= TIMEOUT && client->actual_pid > 0)
	{
		log_msg(LOG_WARNING, "Client timeout reached after %d seconds, initiating cleanup", TIMEOUT);
		clean_global();
	}
	return (0);
}

void	keep_server_up(void)
{
	while (1)
		check_clean_status();
}