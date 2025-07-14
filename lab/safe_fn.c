#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int global_counter = 0;
volatile sig_atomic_t signal_received = 0;

// Signal-safe integer to string conversion
int safe_itoa(int num, char *buf, int buf_size) {
    if (buf_size < 2) return 0;
    
    int is_negative = 0;
    int pos = 0;
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    if (num == 0) {
        buf[pos++] = '0';
    } else {
        char temp[20];
        int temp_pos = 0;
        
        while (num > 0 && temp_pos < 19) {
            temp[temp_pos++] = '0' + (num % 10);
            num /= 10;
        }
        
        if (is_negative && pos < buf_size - 1) {
            buf[pos++] = '-';
        }
        
        for (int i = temp_pos - 1; i >= 0 && pos < buf_size - 1; i--) {
            buf[pos++] = temp[i];
        }
    }
    
    buf[pos] = '\0';
    return pos;
}

// Signal handler (only uses async-signal-safe functions)
void signal_handler(int sig) {
    const char msg[] = "\nSignal received, exiting safely...\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    signal_received = 1;
}

void increment_operation(void)
{
    char buf[100];
    const char prefix[] = "[INC] Counter: ";
    const char suffix[] = "\n";
    
    global_counter++;
    
    // Build string using only signal-safe operations
    int pos = 0;
    
    // Copy prefix
    for (int i = 0; prefix[i] != '\0' && pos < sizeof(buf) - 1; i++) {
        buf[pos++] = prefix[i];
    }
    
    // Convert number to string (signal-safe)
    char num_str[20];
    int num_len = safe_itoa(global_counter, num_str, sizeof(num_str));
    
    // Append number
    for (int i = 0; i < num_len && pos < sizeof(buf) - 1; i++) {
        buf[pos++] = num_str[i];
    }
    
    // Append suffix
    for (int i = 0; suffix[i] != '\0' && pos < sizeof(buf) - 1; i++) {
        buf[pos++] = suffix[i];
    }
    
    // write() is async-signal-safe
    write(STDOUT_FILENO, buf, pos);
    
    // usleep() is NOT async-signal-safe, but we'll use it carefully
    // In a truly signal-safe version, you'd implement a signal-safe delay
    usleep(10000);
}

void decrement_operation(void)
{
    char buf[100];
    const char prefix[] = "[DEC] Counter: ";
    const char suffix[] = "\n";
    
    global_counter--;
    
    // Build string using only signal-safe operations
    int pos = 0;
    
    // Copy prefix
    for (int i = 0; prefix[i] != '\0' && pos < sizeof(buf) - 1; i++) {
        buf[pos++] = prefix[i];
    }
    
    // Convert number to string (signal-safe)
    char num_str[20];
    int num_len = safe_itoa(global_counter, num_str, sizeof(num_str));
    
    // Append number
    for (int i = 0; i < num_len && pos < sizeof(buf) - 1; i++) {
        buf[pos++] = num_str[i];
    }
    
    // Append suffix
    for (int i = 0; suffix[i] != '\0' && pos < sizeof(buf) - 1; i++) {
        buf[pos++] = suffix[i];
    }
    
    // write() is async-signal-safe
    write(STDOUT_FILENO, buf, pos);
    
    // usleep() is NOT async-signal-safe, but we'll use it carefully
    usleep(10000);
}

int main(void)
{
    // Set up signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    const int iterations = 100; // Control the number of operations
    
    // Alternating increment and decrement operations
    for (int i = 0; i < iterations && !signal_received; i++) {
        increment_operation();
        if (signal_received) break;
        decrement_operation();
    }
    
    // Final output using signal-safe functions
    const char final_msg[] = "Final counter value: ";
    write(STDOUT_FILENO, final_msg, sizeof(final_msg) - 1);
    
    char num_str[20];
    int num_len = safe_itoa(global_counter, num_str, sizeof(num_str));
    write(STDOUT_FILENO, num_str, num_len);
    write(STDOUT_FILENO, "\n", 1);
    
    return 0;
}