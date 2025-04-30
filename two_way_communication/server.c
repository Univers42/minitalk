#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

// Global variables
pid_t g_client_pid = 0;
volatile sig_atomic_t g_bit_received = 0;

// Function prototypes
void send_char_to_client(pid_t client_pid, unsigned char c);
void bit_handler(int signum);

// Function to send a character bit by bit to client
void send_char_to_client(pid_t client_pid, unsigned char c)
{
    int i;
    
    printf("Server: Sending char '%c' (0x%02x) to client\n", c > 31 ? c : '.', c);
    
    for (i = 7; i >= 0; i--)
    {
        // Reset acknowledgment flag
        g_bit_received = 0;
        
        // Send bit
        if ((c >> i) & 1)
            kill(client_pid, SIGUSR2);
        else
            kill(client_pid, SIGUSR1);
        
        // Wait for acknowledgment with timeout
        int timeout = 0;
        while (!g_bit_received && timeout < 1000) {
            struct timespec ts = {0, 10000};  // 10 microseconds
            nanosleep(&ts, NULL);
            timeout++;
        }
        
        if (timeout >= 1000)
            printf("Server: Acknowledgment timeout for bit %d\n", i);
    }
}

// Signal handler for all signals
void bit_handler(int signum)
{
    static unsigned char c = 0;
    static int bits_received = 0;
    static char message[4096] = {0};
    static int msg_index = 0;
    
    if (signum == SIGUSR1 && g_client_pid != 0) {
        // Acknowledgment from client
        g_bit_received = 1;
        return;
    }
    
    // Add bit to character based on signal type
    c = (c << 1) | (signum == SIGUSR2 ? 1 : 0);
    bits_received++;
    
    // When full character is received
    if (bits_received == 8)
    {
        // If EOT, exit chat
        if (c == 4)
        {
            printf("\nClient disconnected.\n");
            g_client_pid = 0;
        }
        // If null terminator, process complete message
        else if (c == '\0')
        {
            // First message is client PID
            if (g_client_pid == 0)
            {
                g_client_pid = atoi(message);
                printf("Client connected with PID: %d\n", g_client_pid);
                msg_index = 0;
                message[0] = '\0';
            }
            else
            {
                printf("\nClient: %s", message);
                
                // Get and send response
                char response[4096];
                printf("You: ");
                fgets(response, 4096, stdin);
                fflush(stdout);
                
                // Send response
                char *ptr = response;
                while (*ptr)
                    send_char_to_client(g_client_pid, *ptr++);
                send_char_to_client(g_client_pid, '\0');
                
                // Reset message buffer
                msg_index = 0;
                message[0] = '\0';
            }
        }
        else
        {
            // Add character to message buffer
            message[msg_index++] = c;
            message[msg_index] = '\0';
        }
        
        c = 0;
        bits_received = 0;
    }
    
    // Send acknowledgment to client
    if (g_client_pid != 0)
        kill(g_client_pid, SIGUSR1);
}

int main(void)
{
    // Set up signal handlers
    struct sigaction sa;
    
    // Initialize signal action structure
    sa.sa_handler = bit_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaddset(&sa.sa_mask, SIGUSR2);
    
    // Register the same handler for both signals
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    
    printf("Server started. PID: %d\n", getpid());
    printf("Waiting for client connection...\n");
    
    // Wait for messages indefinitely
    while (1)
        pause();
    
    return 0;
}