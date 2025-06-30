/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 19:32:46 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/30 19:59:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
	Instead of calling spagetti code ft ft_printf, let's recode logs function
	that give more send to a debug maintainability in both client and server.
 */
# include "minitalk.h"

static void	buffer_append(t_string buffer, int *pos, const char *str)
{
	while (*str && *pos < 1023)
		buffer[(*pos)++] = *str++;
}

static inline void	buffer_append_number(t_string buffer, int *pos, int num)
{
	char	temp[12];
	int		i;
	int		is_negative;

	i = 0;
	is_negative = 0;
	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}
	if (num == 0)
		temp[i++] = '0';
	else
	{
		while (num > 0)
		{
			temp[i++] = (num % 10) + '0';
			num /= 10;
		}
	}
	if (is_negative && *pos < 1023)
		buffer[(*pos)++] = '-';
	while (i > 0 && *pos < 1023)
		buffer[(*pos)++] = temp[--i];
}

static inline void	put_level_buffered(t_string buffer, int *pos,
										t_log_level level)
{
	if (level == LOG_INFO)
		buffer_append(buffer, pos, BLUE "[INFO] " RESET);
	else if (level == LOG_WARNING)
		buffer_append(buffer, pos, YELLOW "[WARNING] " RESET);
	else if (level == LOG_ERROR)
		buffer_append(buffer, pos, RED "[ERROR] " RESET);
	else if (level == LOG_DEBUG)
		buffer_append(buffer, pos, MAGENTA "[DEBUG] " RESET);
	else if (level == LOG_SUCCESS)
		buffer_append(buffer, pos, GREEN "[SUCCESS] " RESET);
	else
		buffer_append(buffer, pos, "[UNKNOWN] ");
}

static void	vlog_msg(const char *fmt, va_list args, t_string buffer, int *pos)
{
	int	i;

	i = 0;
	while (fmt[i] && *pos < 1023)
	{
		if (fmt[i] == '%' && fmt[i + 1])
		{
			if (fmt[i + 1] == 's')
			{
				buffer_append(buffer, pos, va_arg(args, char *));
				i += 2;
				continue ;
			}
			else if (fmt[i + 1] == 'd')
			{
				buffer_append_number(buffer, pos, va_arg(args, int));
				i += 2;
				continue ;
			}
		}
		buffer[(*pos)++] = fmt[i];
		i++;
	}
}

// Only supports %s and %d, everything else is printed as-is
void	log_msg(t_log_level level, const char *fmt, ...)
{
	char		buffer[1024];
	va_list		args;
	t_flag		enabled;
	int			pos;

	pos = 0;
	enabled = MINITALK_DEBUG;
	if (!enabled)
		return ;
	put_level_buffered(buffer, &pos, level);
	va_start(args, fmt);
	vlog_msg(fmt, args, buffer, &pos);
	buffer[pos++] = '\n';
	buffer[pos] = '\0';
	va_end(args);
	write(1, buffer, pos);
}