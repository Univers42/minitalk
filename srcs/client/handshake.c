/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handshake.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:15:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 18:23:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

#define AVAILABLE 1
#define BUSY 0
#define ERROR -1

/* Configuration constants */
#define DEFAULT_POLL_INTERVAL_US 50000
#define DEFAULT_RETRY_DELAY_MS 100
#define MAX_RETRIES 10

typedef enum e_handshake_result
{
	HS_SUCCESS = 1,
	HS_TIMEOUT = 0,
	HS_ERROR = -1,
	HS_INVALID_PARAMS = -2,
	HS_SERVER_DEAD = -3
}	t_handshake_result;

typedef struct s_handshake_config
{
	int	timeout_ms;
	int	poll_interval_us;
	int	max_retries;
	int	retry_delay_ms;
}	t_handshake_config;

typedef struct s_handshake
{
	pid_t						server_pid;
	volatile sig_atomic_t		is_ready;
	int							ready_signal;
	int							busy_signal;
	int							ping_signal;
	t_handshake_config			config;
}	t_handshake;

/* ========== SINGLETON MANAGEMENT ========== */

static t_handshake	*get_instance(void)
{
	static t_handshake	instance = {0};

	return (&instance);
}

/* ========== VALIDATION FUNCTIONS ========== */

static int	is_valid_pid(pid_t pid)
{
	return (pid > 0);
}

static int	is_valid_signal(int sig)
{
	return (sig > 0 && sig < NSIG);
}

static int	is_valid_timeout(int timeout_ms, int poll_interval_us)
{
	return (timeout_ms > 0 && poll_interval_us > 0
		&& poll_interval_us <= timeout_ms * 1000);
}

static int	validate_config(const t_handshake_config *config)
{
	if (!config)
		return (0);
	return (is_valid_timeout(config->timeout_ms, config->poll_interval_us)
		&& config->max_retries > 0 && config->retry_delay_ms >= 0);
}

/* ========== PROCESS MANAGEMENT ========== */

int	is_process_alive(pid_t pid)
{
	if (!is_valid_pid(pid))
		return (0);
	return (kill(pid, 0) == 0);
}

static int	send_signal_safe(pid_t pid, int sig)
{
	if (!is_process_alive(pid) || !is_valid_signal(sig))
		return (HS_SERVER_DEAD);
	if (kill(pid, sig) == -1)
		return (HS_ERROR);
	return (HS_SUCCESS);
}

/* ========== SIGNAL HANDLING ========== */

static void	signal_handler(int signum, siginfo_t *info, void *context)
{
	t_handshake	*hs;

	(void)context;
	hs = get_instance();
	if (info->si_pid != hs->server_pid)
		return ;
	if (signum == hs->ready_signal)
		hs->is_ready = AVAILABLE;
	else if (signum == hs->busy_signal)
		hs->is_ready = BUSY;
}

static int	setup_signal_handler(int signal, struct sigaction *sa)
{
	if (!is_valid_signal(signal))
		return (HS_INVALID_PARAMS);
	return (sigaction(signal, sa, NULL) == -1 ? HS_ERROR : HS_SUCCESS);
}

static int	configure_signals(void)
{
	struct sigaction	sa;
	sigset_t			mask;
	t_handshake			*hs;

	hs = get_instance();
	if (sigemptyset(&mask) == -1
		|| sigaddset(&mask, hs->ready_signal) == -1
		|| sigaddset(&mask, hs->busy_signal) == -1)
		return (HS_ERROR);
	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_mask = mask;
	if (setup_signal_handler(hs->ready_signal, &sa) != HS_SUCCESS
		|| setup_signal_handler(hs->busy_signal, &sa) != HS_SUCCESS)
		return (HS_ERROR);
	return (HS_SUCCESS);
}

/* ========== TIMING UTILITIES ========== */

static int	sleep_interruptible(int microseconds)
{
	if (usleep(microseconds) == -1 && errno != EINTR)
		return (HS_ERROR);
	return (HS_SUCCESS);
}

static int	wait_with_timeout(int timeout_us, int poll_us)
{
	int			elapsed_us;
	t_handshake	*hs;

	elapsed_us = 0;
	hs = get_instance();
	while (elapsed_us < timeout_us)
	{
		if (hs->is_ready == AVAILABLE)
			return (HS_SUCCESS);
		if (sleep_interruptible(poll_us) == HS_ERROR)
			return (HS_ERROR);
		elapsed_us += poll_us;
	}
	return (HS_TIMEOUT);
}

/* ========== CORE HANDSHAKE FUNCTIONS ========== */

int	handshake_init(pid_t pid, int ready_sig, int busy_sig, int ping_sig)
{
	t_handshake	*hs;

	if (!is_valid_pid(pid) || !is_valid_signal(ready_sig)
		|| !is_valid_signal(busy_sig) || !is_valid_signal(ping_sig))
		return (HS_INVALID_PARAMS);
	hs = get_instance();
	hs->server_pid = pid;
	hs->ready_signal = ready_sig;
	hs->busy_signal = busy_sig;
	hs->ping_signal = ping_sig;
	hs->is_ready = BUSY;
	hs->config = (t_handshake_config){2000, DEFAULT_POLL_INTERVAL_US,
		3, DEFAULT_RETRY_DELAY_MS};
	return (configure_signals());
}

void	handshake_cleanup(void)
{
	t_handshake	*hs;

	hs = get_instance();
	if (is_valid_signal(hs->ready_signal))
		signal(hs->ready_signal, SIG_DFL);
	if (is_valid_signal(hs->busy_signal))
		signal(hs->busy_signal, SIG_DFL);
}

void	handshake_reset(void)
{
	t_handshake	*hs;

	hs = get_instance();
	hs->is_ready = BUSY;
}

int	handshake_is_ready(void)
{
	t_handshake	*hs;

	hs = get_instance();
	return (hs->is_ready == AVAILABLE);
}

int	handshake_wait(int timeout_ms, int poll_interval_us)
{
	if (!is_valid_timeout(timeout_ms, poll_interval_us))
		return (HS_INVALID_PARAMS);
	return (wait_with_timeout(timeout_ms * 1000, poll_interval_us));
}

int	handshake_ping(void)
{
	t_handshake	*hs;

	hs = get_instance();
	return (send_signal_safe(hs->server_pid, hs->ping_signal));
}

/* ========== RETRY MECHANISMS ========== */

static int	single_attempt(int with_ping, int timeout_ms, int poll_us)
{
	int	result;

	handshake_reset();
	if (with_ping)
	{
		result = handshake_ping();
		if (result != HS_SUCCESS)
			return (result);
	}
	return (handshake_wait(timeout_ms, poll_us));
}

int	handshake_retry(int max_retries, int timeout_ms, int poll_us)
{
	int	attempt;
	int	result;

	if (max_retries <= 0 || !is_valid_timeout(timeout_ms, poll_us))
		return (HS_INVALID_PARAMS);
	attempt = 0;
	while (attempt < max_retries)
	{
		result = single_attempt(0, timeout_ms, poll_us);
		if (result == HS_SUCCESS || result == HS_ERROR)
			return (result);
		if (++attempt < max_retries)
			sleep_interruptible(DEFAULT_RETRY_DELAY_MS * 1000);
	}
	return (HS_TIMEOUT);
}

int	handshake_retry_with_ping(int max_retries, int timeout_ms, int poll_us)
{
	int	attempt;
	int	result;

	if (max_retries <= 0 || !is_valid_timeout(timeout_ms, poll_us))
		return (HS_INVALID_PARAMS);
	attempt = 0;
	while (attempt < max_retries)
	{
		result = single_attempt(1, timeout_ms, poll_us);
		if (result == HS_SUCCESS || result == HS_ERROR
			|| result == HS_SERVER_DEAD)
			return (result);
		if (++attempt < max_retries)
			sleep_interruptible(DEFAULT_RETRY_DELAY_MS * 1000);
	}
	return (HS_TIMEOUT);
}

/* ========== CONFIGURATION ========== */

int	handshake_set_config(const t_handshake_config *config)
{
	t_handshake	*hs;

	if (!validate_config(config))
		return (HS_INVALID_PARAMS);
	hs = get_instance();
	hs->config = *config;
	return (HS_SUCCESS);
}

int	handshake_get_config(t_handshake_config *config)
{
	t_handshake	*hs;

	if (!config)
		return (HS_INVALID_PARAMS);
	hs = get_instance();
	*config = hs->config;
	return (HS_SUCCESS);
}