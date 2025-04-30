#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>

// Global variables
pid_t g_server_pid;
volatile sig_atomic_t g_bit_received = 0;
volatile sig_atomic_t g_msg_complete = 0;

// Function prototypes
void send_char(pid_t server_pid, unsigned char c);
void bit_handler(int signum);
void chat_loop(pid_t server_pid);

// Function to send a character bit by bit to server
void send_char(pid_t server_pid, unsigned char c)
{
    int i;
    
    printf("Client: Sending char '%c' (0x%02x) to server\n", c > 31 ? c : '.', c);
    
    for (i = 7; i >= 0; i--)
    {
        // Reset acknowledgment flag
        g_bit_received = 0;
        
        // Send bit
        if ((c >> i) & 1)
            kill(server_pid, SIGUSR2);
        else
            kill(server_pid, SIGUSR1);
        
        // Wait for acknowledgment with timeout
        int timeout = 0;
        while (!g_bit_received && timeout < 1000) {
            struct timespec ts = {0, 10000};  // 10 microseconds
            nanosleep(&ts, NULL);
            timeout++;
        }
        
        if (timeout >= 1000)
            printf("Client: Acknowledgment timeout for bit %d\n", i);
    }
}

// Signal handler for all signals
void bit_handler(int signum)
{
    static unsigned char c = 0;
    static int bits_received = 0;
    static char message[4096] = {0};
    static int msg_index = 0;
    
    if (signum == SIGUSR1) {
        // Handle acknowledgment
        g_bit_received = 1;
        return;
    }
    
    // Add bit to character based on signal type (SIGUSR2 = 1)
    c = (c << 1) | 1;
    bits_received++;
    
    // When full character is received
    if (bits_received == 8)
    {
        if (c == '\0')
        {
            write(STDOUT_FILENO, "\nServer: ", 9);
            write(STDOUT_FILENO, message, msg_index);
            write(STDOUT_FILENO, "\n", 1);
            g_msg_complete = 1;
            msg_index = 0;
            message[0] = '\0';
        }
        else
        {
            message[msg_index++] = c;
            message[msg_index] = '\0';
        }
        
        c = 0;
        bits_received = 0;
    }
    
    // Acknowledge bit receipt
    kill(g_server_pid, SIGUSR1);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <server_pid>\n", argv[0]);
        return 1;
    }
    
    // Store server PID
    g_server_pid = atoi(argv[1]);
    if (g_server_pid <= 0)
    {
        printf("Invalid server PID\n");
        return 1;
    }
    
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
    
    // Send client PID to server
    printf("Client started. PID: %d\n", getpid());
    printf("Connected to server with PID: %d\n", g_server_pid);
    
    // Initial handshake - send client PID as a string
    char pid_str[32];
    sprintf(pid_str, "%d", getpid());
    char *ptr = pid_str;
    while (*ptr)
        send_char(g_server_pid, *ptr++);
    send_char(g_server_pid, '\0');
    
    // Enter chat loop
    chat_loop(g_server_pid);
    
    printf("Chat ended\n");
    return 0;
}

void chat_loop(pid_t server_pid)
{
    char input[4096];
    
    while (1)
    {
        printf("\nYou: ");
        fflush(stdout);
        fgets(input, 4096, stdin);
        
        // Exit condition
        if (strcmp(input, "exit\n") == 0)
            break;
        
        // Send message
        char *ptr = input;
        while (*ptr)
            send_char(server_pid, *ptr++);
        send_char(server_pid, '\0');
        
        // Wait for message completion
        g_msg_complete = 0;
        printf("Waiting for server response...\n");
        while (!g_msg_complete) {
            pause();
        }
    }
    
    // Send exit notification
    send_char(server_pid, 4);  // EOT character
}