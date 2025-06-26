/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:47:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 12:56:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_bonus.h"

static void	handle_sig(int sig, siginfo_t *info, void *context);

int	main(void)
{
	t_server	*srv;
	t_sigaction	sa;
	t_sigaction	sa_cleanup;

	ft_printf("Server bonus is running...\nPID: %d\n", getpid());
	srv = get_server();
	memset(srv, 0, sizeof(t_server));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handle_sig;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigemptyset(&sa_cleanup.sa_mask);
	sa_cleanup.sa_flags = 0;
	sa_cleanup.sa_handler = clenaup_still_reachable;
	sigaction(SIGINT, &sa_cleanup, NULL);
	sigaction(SIGTERM, &sa_cleanup, NULL);
	while (1)
		pause();
	return (0);
}

static void	flush(t_server *srv, siginfo_t *info, int wrap_flag)
{
	if (wrap_flag == 1)
	{
		srv->buf_i = 0;
		srv->bit_i = 0;
		srv->cur_byte = 0;
		srv->last_client = info->si_pid;
	}
	if (wrap_flag == 2)
	{
		free(srv->msg_buf);
		ft_memset(srv, 0, sizeof(t_server));
	}
}

static void	grow_buffer(t_server *srv, siginfo_t *info)
{
	t_string	new_buf;

	if (srv->buf_i + 2 > srv->buf_cap)
	{
		new_buf = ft_realloc(srv->msg_buf, srv->buf_cap, srv->buf_cap * 2);
		if (!new_buf)
		{
			flush(srv, info, SECOND_FLUSH);
			return ;
		}
		srv->msg_buf = new_buf;
		srv->buf_cap *= 2;
	}
	srv->msg_buf[srv->buf_i++] = srv->cur_byte;
	if (srv->cur_byte == '\0')
	{
		ft_printf("Received: %s\n", srv->msg_buf);
		flush(srv, info, SECOND_FLUSH);
	}
	srv->cur_byte = 0;
	srv->bit_i = 0;
	kill(info->si_pid, SIGUSR1);
}

static void	handle_sig(int sig, siginfo_t *info, void *context)
{
	t_server	*srv;

	(void)context;
	srv = get_server();
	if (!srv->msg_buf)
	{
		srv->buf_cap = MSG_BUF_SIZE;
		srv->msg_buf = malloc(srv->buf_cap);
		if (srv->msg_buf)
			memset(srv->msg_buf, 0, srv->buf_cap);
		if (!srv->msg_buf)
			return ;
	}
	if (srv->last_client != info->si_pid)
		flush(srv, info, FIRST_FLUSH);
	if (sig == SIGUSR1)
		srv->cur_byte |= (1 << srv->bit_i);
	kill(info->si_pid, SIGUSR1);
	srv->bit_i++;
	if (srv->bit_i == SEQ_BIT)
		grow_buffer(srv, info);
}
