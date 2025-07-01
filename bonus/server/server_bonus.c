/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 15:47:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_bonus.h"

static void	handle_sig(int sig, siginfo_t *info, void *context);

int	main(void)
{
	t_server			*srv;
	t_sigaction			sa;
	t_sigaction			sa_cleanup;

	ft_printf("Server bonus is running...\nPID: %d\n", getpid());
	srv = get_server();
	ft_memset(srv, 0, sizeof(t_server));
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO | SA_NODEFER;
	sa.sa_sigaction = handle_sig;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigemptyset(&sa_cleanup.sa_mask);
	sa_cleanup.sa_flags = 0;
	sa_cleanup.sa_handler = cleanup_still_reachable;
	sigaction(SIGINT, &sa_cleanup, NULL);
	sigaction(SIGTERM, &sa_cleanup, NULL);
	while (1)
		pause();
	return (0);
}

static void	flush(t_server *srv, pid_t client_pid, int wrap_flag)
{
	if (wrap_flag == 1)
	{
		srv->buf_i = 0;
		srv->bit_i = 0;
		srv->cur_byte = 0;
		srv->last_client = client_pid;
	}
	if (wrap_flag == 2)
	{
		if (srv->msg_buf)
			free(srv->msg_buf);
		ft_memset(srv, 0, sizeof(t_server));
	}
}

static void	grow_buffer(t_server *srv, pid_t client_pid)
{
	t_string	new_buf;

	if (srv->buf_i + 2 > srv->buf_cap)
	{
		new_buf = ft_realloc(srv->msg_buf, srv->buf_cap * 2, srv->buf_cap);
		if (!new_buf)
			return (flush(srv, client_pid, SECOND_FLUSH));
		srv->msg_buf = new_buf;
		srv->buf_cap *= 2;
	}
	srv->msg_buf[srv->buf_i++] = srv->cur_byte;
	if (srv->cur_byte == '\0')
	{
		ft_printf("Received: %s\n", srv->msg_buf);
		flush(srv, client_pid, SECOND_FLUSH);
	}
	srv->cur_byte = 0;
	srv->bit_i = 0;
	kill(client_pid, SIGUSR1);
}

static int	initialize_server_buffer(t_server *srv)
{
	if (!srv->msg_buf)
	{
		srv->buf_cap = MSG_BUF_SIZE;
		srv->msg_buf = malloc(srv->buf_cap);
		if (srv->msg_buf)
			ft_memset(srv->msg_buf, 0, srv->buf_cap);
		if (!srv->msg_buf)
			return (0);
	}
	return (1);
}

static void	handle_sig(int sig, siginfo_t *info, void *context)
{
	static t_sig_atomic	processing = 0;
	t_server			*srv;

	(void)context;
	if (processing)
		return ;
	processing = 1;
	srv = get_server();
	if (!initialize_server_buffer(srv))
	{
		processing = 0;
		return ;
	}
	if (srv->last_client != info->si_pid)
		flush(srv, info->si_pid, FIRST_FLUSH);
	if (sig == SIGUSR1)
		srv->cur_byte |= (1 << srv->bit_i);
	srv->bit_i++;
	if (srv->bit_i == SEQ_BIT)
		grow_buffer(srv, info->si_pid);
	kill(info->si_pid, SIGUSR1);
	processing = 0;
}
