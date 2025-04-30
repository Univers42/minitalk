#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void handler(int signum) {
    printf("Signal %d received! Handler runs once only.\n", signum);
    // Signal disposition is automatically reset to SIG_DFL
    // Need to re-register handler for future signals
    signal(signum, handler);
    
    // In System V, if another SIGINT happens during this handler,
    // it would interrupt this handler (no blocking)
    sleep(3);
    printf("Handler completed\n");
}

//int main(void) {
//    // Explicitly request System V semantics
//    struct sigaction sa;
//    sa.sa_handler = handler;
//    sigemptyset(&sa.sa_mask);
//    sa.sa_flags = SA_RESETHAND | SA_NODEFER;  // System V behavior
//    sigaction(SIGINT, &sa, NULL);
//    
//    printf("Press Ctrl+C multiple times quickly...\n");
//    while(1) {
//        sleep(1);
//    }
//}

int main(void) {
    // BSD semantics
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;  // BSD behavior
    sigaction(SIGINT, &sa, NULL);
    
    printf("Press Ctrl+C multiple times quickly...\n");
    while(1) {
        sleep(1);
    }
}