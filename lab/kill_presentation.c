/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_presentation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:06:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/14 23:55:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static volatile sig_atomic_t running = 1;

void handle_signal(int sig)
{
    printf("\n[SIGNAL] Received signal %d\n", sig);
    running = 0;
}

void version_signal(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGHUP, handle_signal);
}

void version_sigaction(void)
{
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
}

int main(int argc, char **argv)
{
    pid_t server_pid;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [signal|sigaction]\n", argv[0]);
        return 1;
    }

    server_pid = getpid();
    printf("[Signal] PID: %d\n", server_pid);
    printf("Try: kill -SIGTERM %d or kill -SIGINT %d\n", server_pid, server_pid);

    if (strcmp(argv[1], "sigaction") == 0)
        version_sigaction();
    else if (strcmp(argv[1], "signal") == 0)
        version_signal();
    else
    {
        fprintf(stderr, "Unknown option: %s. Use 'signal' or 'sigaction'.\n", argv[1]);
        return 1;
    }

    while (running)
        pause();

    printf("[Signal] Exiting cleanly.\n");
    return 0;
}
