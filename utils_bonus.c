/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:03:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/26 12:03:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_bonus.h"

void    clenaup_still_reachable(int sig)
{
    t_server *srv = get_server();

    if (srv->msg_buf)
        free(srv->msg_buf);
    ft_printf("Server exiting on signal %d\n", sig);
    exit(0);
}

t_server *get_server(void)
{
    static t_server server;
    return &server;
}

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void *new_ptr;

    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;
    if (ptr)
    {
        size_t copy_size = old_size < new_size ? old_size : new_size;
        memcpy(new_ptr, ptr, copy_size);
        free(ptr);
    }
    return new_ptr;
}

