/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:45:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 19:30:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include "libft/libft.h"
#include <sys/types.h>
#include <signal.h>
#include "log.h"
#define BUFFER_SIZE 1024

typedef struct s_client_state
{
	pid_t client_pid;
	unsigned char byte;
	int bit_index;
	int byte_index;
	char *buffer;
	size_t buffer_size;
	struct s_client_state *next;
} t_client_state;

void handle_signal(int signal, siginfo_t *info, void *context);
void init_server(void);

#endif