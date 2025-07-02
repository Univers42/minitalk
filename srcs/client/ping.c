#include "client.h"

#define RETRY_TIMES 10
#define RETRY_TIME 1
#define RETRY_INTERVALS 20

#define SERVER_READY SIGUSR1
#define SERVER_BUSY SIGUSR2

void	ping_handler(int signum, siginfo_t *info, void *context)
{
	t_server_state	*server;
	const char		*signal_name;

	(void)context;
	server = get_server_instance();
	signal_name = (signum == SIGUSR1) ? "SIGUSR1 (READY)" : "SIGUSR2 (BUSY)";
	
	log_msg(LOG_DEBUG, "Ping handler received %s from PID %d", signal_name, info->si_pid);
	
	if (info->si_pid == getpid())
	{
		log_msg(LOG_ERROR, "Received signal from own process");
		exit(EXIT_FAILURE);
	}
	if (server->pid != 0 && info->si_pid != server->pid)
	{
		log_msg(LOG_WARNING, "Unexpected PID in ping_handler: %d (expected %d)", info->si_pid, server->pid);
		return;
	}
	if (signum == SERVER_READY)
	{
		server->is_ready = 1;
		log_msg(LOG_SUCCESS, "Server ready signal received from PID %d", info->si_pid);
	}
	if (signum == SERVER_BUSY)
	{
		server->is_ready = 0;
		log_msg(LOG_INFO, "Server busy signal received from PID %d", info->si_pid);
	}
}

int	check_server_and_sleep(void)
{
	const int		total_sleep = RETRY_TIME * 1000 * 1000;
	const int		sleep_time = total_sleep / RETRY_INTERVALS;
	int				i;
	t_server_state	*server;

	server = get_server_instance();
	i = 0;
	while (i <= total_sleep)
	{
		usleep(sleep_time);
		if (server->is_ready == 1)
			return (1);
		i += sleep_time;
	}
	return (0);
}

int	handle_timeouts(int pid)
{
	int	i;

	i = 0;
	log_msg(LOG_INFO, "Starting ping sequence to server PID %d", pid);
	while (++i <= RETRY_TIMES)
	{
		log_msg(LOG_DEBUG, "Sending ping signal %d/%d to server", i, RETRY_TIMES);
		kill(pid, SIGUSR1);
		log_msg(LOG_INFO, "Waiting for server response (attempt %d/%d)", i, RETRY_TIMES);
		if (check_server_and_sleep())
		{
			log_msg(LOG_SUCCESS, "Server responded on attempt %d", i);
			return (0);
		}
		log_msg(LOG_WARNING, "No response on attempt %d, retrying...", i);
	}
	log_msg(LOG_ERROR, "Server did not respond after %d attempts", RETRY_TIMES);
	return (1);
}

int	ping(int pid)
{
	struct sigaction	sa;
	sigset_t			sigset;
	t_server_state		*server;

	server = get_server_instance();
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = ping_handler;
	sa.sa_mask = sigset;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	server->pid = pid;
	server->is_ready = 0;
	if (handle_timeouts(pid))
	{
		ft_printf("Error: Couldn't reach server PID %d\n", pid);
		log_msg(LOG_ERROR, "Server ping timeout");
		return (0);
	}
	ft_printf("Server ready, sending message.\n");
	log_msg(LOG_SUCCESS, "Server connection established");
	return (1);
}