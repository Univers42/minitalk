/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minitalk.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:15:45 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 23:28:54 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINITALK_H
# define FT_MINITALK_H
# define _POSIX_C_SOURCE 200809L
# define _DEFAULT_SOURCE
# define _XOPEN_SOURCE 700

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/time.h>

# define SIGNAL_DELAY 50

typedef struct s_data {
	char			c;
	int				bit_index;
	pid_t			client_pid;
	struct timeval	start_time;
	struct timeval	end_time;
	int				char_count;
}	t_data;

#endif