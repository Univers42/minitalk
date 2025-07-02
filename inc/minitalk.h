/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/02 19:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include "libft.h"
# include <signal.h>
# include <unistd.h>
# include <sys/types.h>

/* Global variable for acknowledgment */
extern volatile sig_atomic_t g_ack_received;

/* Function prototypes */
void	client_signal_handler(int sig);
void	server_signal_handler(int sig, siginfo_t *info, void *context);
int		send_bit(pid_t server_pid, int bit);
int		send_byte(pid_t server_pid, unsigned char byte);
int		send_message_minitalk(pid_t server_pid, char *message);

/* Add missing NSIG definition if not available */
#ifndef NSIG
# define NSIG 64
#endif

#endif