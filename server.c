/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:08:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 13:27:14 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server implementation for minitalk
#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global variables
static pid_t g_client_pid = 0;

// Signal handler for received bits
void sig_handler(int signum)
{
    static unsigned char c = 0;
    static int bits_received = 0;
    
    // Add bit to character based on signal type
    c = (c << 1) | (signum == SIGUSR2 ? 1 : 0);
    bits_received++;
    
    // When full character is received
    if (bits_received == 8)
    {
        // If null terminator, print newline to indicate end of message
        if (c == '\0')
            write(1, "\n", 1);
        else
            write(1, &c, 1);
        
        // Reset for next character
        c = 0;
        bits_received = 0;
    }
    
    // Send acknowledgment to client
    if (g_client_pid != 0)
        kill(g_client_pid, SIGUSR1);
}

// Handler for client PID reception (SIGINT)
void client_pid_handler(int signum, siginfo_t *info, void *context)
{
    (void)signum;
    (void)context;
    
    // Store client PID for acknowledgments
    g_client_pid = info->si_pid;
}

int main(void)
{
    struct sigaction sa;
    struct sigaction sa_info;
    
    // Print server PID
    printf("Server PID: %d\n", getpid());
    
    // Set up signal handler for bits
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    // Set up signal handler with info for getting client PID
    sa_info.sa_sigaction = client_pid_handler;
    sigemptyset(&sa_info.sa_mask);
    sa_info.sa_flags = SA_SIGINFO;
    
    // Register handlers
    sigaction(SIGUSR1, &sa, NULL);      // For bit 0
    sigaction(SIGUSR2, &sa, NULL);      // For bit 1
    sigaction(SIGINT, &sa_info, NULL);  // For client PID (optional)
    
    // Wait for signals indefinitely
    while (1)
        pause();
    
    return 0;
}