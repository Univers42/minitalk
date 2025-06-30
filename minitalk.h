/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:16:17 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 19:38:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H
# include "libft/libft.h"
# include <stdarg.h>

# ifndef MINITALK_DEBUG
#  define MINITALK_DEBUG 0
# endif

typedef enum e_log_level
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG,
    LOG_SUCCESS
}               t_log_level;

void    log_msg(t_log_level level, const char *fmt, ...);


#endif