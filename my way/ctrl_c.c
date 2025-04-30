/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:00:04 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 23:00:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_minitalk.h"
void handler(int signum)
{
    printf("Received signal %d\n", signum);;
}

int main(void)
{
    struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction failed");
        return (1);
    }
    //signal(SIGTERM, handler);
    //signal(SIGUSR1, handler);
    //signal(SIGUSR2, handler);

    while (1)
    {
        printf("Running...\n");
        sleep(1);
    }

    return 0;
}