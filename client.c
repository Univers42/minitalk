/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:08:47 by dlesieur          #+#    #+#             */
/*   Updated: 2025/04/29 13:34:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Client example showing signal usage
#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
// Signal handler for acknowledgments from server
void sig_handler(int signum)
{
    if (signum == SIGUSR1)
        printf("Bit received by server\n");
}

// Function to send a character bit by bit
void send_char(pid_t server_pid, unsigned char c)
{
    int i;
    
    // Process each bit, from MSB to LSB
    for (i = 7; i >= 0; i--)
    {
        // Check if bit is 0 or 1
        if ((c >> i) & 1)
            kill(server_pid, SIGUSR2); // Send 1
        else
            kill(server_pid, SIGUSR1); // Send 0
        
        // Wait for acknowledgment (required for reliability)
        struct timespec ts = {0, 100000};  // 100 microseconds
        nanosleep(&ts, NULL);
    }
}

int main(int argc, char **argv)
{
    pid_t server_pid;
    struct sigaction sa;
    char *message;
    
    // Set up signal handler for acknowledgment
    sa.sa_handler = sig_handler;
    
    // Initialize empty signal set for sa_mask
    sigemptyset(&sa.sa_mask);
    
    // Add signals we want to block during handler execution
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);
    
    sa.sa_flags = 0;
    
    // Register handlers using sigactions
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    
    // Alternative using older signal() function (less recommended):
    // signal(SIGUSR1, sig_handler);
    
    if (argc != 3)
    {
        printf("Usage: %s <server_pid> <message>\n", argv[0]);
        return 1;
    }
    
    server_pid = atoi(argv[1]);
    message = argv[2];
    
    printf("Client PID: %d\n", getpid());
    printf("Sending message to server (PID: %d)\n", server_pid);
    
    // Send each character
    while (*message)
        send_char(server_pid, *message++);
        
    // Send null terminator
    send_char(server_pid, '\0');
    
    return 0;
}