/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:53:54 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/27 19:35:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * Instead of calling spaghetti code of ft_printf, let's create a beautiful log
 * function that can be used in both client and server.
 */

#include "log.h"
#include "libft.h"
#include <unistd.h>
#include <stdarg.h>

/*
 * Instead of calling spaghetti code of ft_printf, let's create a beautiful log
 * function that can be used in both client and server.
 */
#include "log.h"
#include "libft.h"
#include <unistd.h>
#include <stdarg.h>

static void put_level(int fd, t_log_level level)
{
    if (level == LOG_INFO)
        ft_putstr_fd("[INFO] ", fd);
    else if (level == LOG_WARNING)
        ft_putstr_fd("[WARNING] ", fd);
    else if (level == LOG_ERROR)
        ft_putstr_fd("[ERROR] ", fd);
    else if (level == LOG_DEBUG)
        ft_putstr_fd("[DEBUG] ", fd);
    else if (level == LOG_SUCCESS)
        ft_putstr_fd("[SUCCESS] ", fd);
    else
        ft_putstr_fd("[UNKNOWN] ", fd);
}

// Only supports %s and %d, everything else is printed as-is
void log_msg(t_log_level level, const char *fmt, ...)
{
    va_list args;
    int i;

    put_level(1, level);
    va_start(args, fmt);
    i = 0;
    while (fmt[i])
    {
        if (fmt[i] == '%' && fmt[i + 1])
        {
            if (fmt[i + 1] == 's')
            {
                ft_putstr_fd(va_arg(args, char *), 1);
                i += 2;
                continue;
            }
            else if (fmt[i + 1] == 'd')
            {
                ft_putnbr_fd(va_arg(args, int), 1);
                i += 2;
                continue;
            }
        }
        write(1, &fmt[i], 1);
        i++;
    }
    va_end(args);
    write(1, "\n", 1);
}