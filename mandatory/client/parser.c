/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:54:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/01 16:42:28 by dlesieur         ###   ########.fr       */
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

int is_pid_alive(pid_t pid)
{
    return (kill(pid, 0) == 0);
}

void    eval()
{
    
}