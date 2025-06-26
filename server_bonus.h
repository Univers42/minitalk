/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:48:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 12:56:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_BONUS_H
# define SERVER_BONUS_H

# include "libft.h"

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

void		*ft_realloc(void *ptr, size_t old_size, size_t new_size);
t_server	*get_server(void);
void		clenaup_still_reachable(int sig);

#endif