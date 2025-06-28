/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:32:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 23:33:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

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
}			t_log_level;

// No struct needed, just use the enum
void	log_msg(t_log_level level, const char *fmt, ...);

#endif