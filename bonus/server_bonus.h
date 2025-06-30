/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 21:11:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_BONUS_H
# define SERVER_BONUS_H

# include "libft.h"
# include <signal.h>
# include <sys/types.h>

# define MSG_BUF_SIZE	5
# define SEQ_BIT		8
# define FIRST_FLUSH	1
# define SECOND_FLUSH	2

typedef struct s_server
{
	t_string		msg_buf;
	t_size			buf_cap;
	t_size			buf_i;
	int				bit_i;
	t_uint8			cur_byte;
	pid_t			last_client;
}					t_server;

/* Function prototypes */
t_server	*get_server(void);
void		cleanup_still_reachable(int sig);

#endif