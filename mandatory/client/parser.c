/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:54:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 17:58:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int is_valid_pid(const char *str)
{
    if (!str || !*str)
        return (0);
    while (*str)
    {
        if (!(ft_isdigit(*str++)))
            return (0);
    }
    return (1);
}

void    eval(const char *str)
{
    const char *ptr = str;
    if (!*ptr || !ft_isdigit(*ptr))
        ft_print_error((char *str))
    while (*++ptr)
    {
        if (!ft_isdigit(*ptr))
            ft_print_error((char *)str);
    }
}

void    parser(int argc, char **argv)
{
    const char *str;

    if (argc != 3)
        log_msg(LOG_ERROR, "usag")
}