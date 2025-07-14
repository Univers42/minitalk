// Demo: volatile sig_atomic_t vs regular variables
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// =============================================================================
// PART 1: Using volatile sig_atomic_t (CORRECT way)
// =============================================================================

volatile sig_atomic_t correct_flag = 0;
volatile sig_atomic_t correct_counter = 0;

void correct_signal_handler(int sig) {
    (void)sig;
    correct_flag = 1;
    correct_counter++;
    
    // Only async-signal-safe functions allowed here
    const char msg[] = "CORRECT: Signal received safely!\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

void test_correct_implementation() {
    printf("\n=== TESTING CORRECT IMPLEMENTATION (volatile sig_atomic_t) ===\n");
    
    // Reset values
    correct_flag = 0;
    correct_counter = 0;
    
    // Install signal handler
    signal(SIGUSR1, correct_signal_handler);
    
    printf("Process PID: %d\n", getpid());
    printf("Send SIGUSR1 to this process: kill -USR1 %d\n", getpid());
    printf("Waiting for signal... (Press Ctrl+C to continue to next test)\n");
    
    // Main loop that checks the flag
    while (correct_counter < 3) {
        // This check is SAFE because:
        // 1. volatile ensures the value is read from memory each time
        // 2. sig_atomic_t ensures atomic read/write operations
        if (correct_flag) {
            printf("Main loop detected flag change! Counter: %d\n", (int)correct_counter);
            
            // Reset flag for next signal
            correct_flag = 0;
            
            // Small delay to show the behavior
            usleep(100000);
        }
        
        // Small delay to prevent busy waiting
        usleep(10000);
    }
    
    printf("Correct implementation completed successfully!\n");
}

// =============================================================================
// PART 2: Using regular variables (PROBLEMATIC way)
// =============================================================================

int problematic_flag = 0;          // NOT volatile, NOT sig_atomic_t
int problematic_counter = 0;       // NOT volatile, NOT sig_atomic_t

void problematic_signal_handler(int sig) {
    (void)sig;
    
    // PROBLEM 1: These operations might not be atomic
    // The compiler might generate multiple assembly instructions
    // for these operations, leading to race conditions
    problematic_flag = 1;
    problematic_counter++;
    
    // PROBLEM 2: Using non-async-signal-safe function
    // This is undefined behavior and can cause crashes
    printf("PROBLEMATIC: Signal received (THIS IS UNSAFE!)\n");
    
    // PROBLEM 3: More complex operations that are definitely not atomic
    problematic_counter = problematic_counter * 2 + 1;
}

void test_problematic_implementation() {
    printf("\n=== TESTING PROBLEMATIC IMPLEMENTATION (regular int) ===\n");
    printf("WARNING: This demonstrates what NOT to do!\n");
    
    // Reset values
    problematic_flag = 0;
    problematic_counter = 0;
    
    // Install signal handler
    signal(SIGUSR2, problematic_signal_handler);
    
    printf("Process PID: %d\n", getpid());
    printf("Send SIGUSR2 to this process: kill -USR2 %d\n", getpid());
    printf("Waiting for signal... (Press Ctrl+C to continue)\n");
    
    int local_counter = 0;
    
    // Main loop that checks the flag
    while (local_counter < 3) {
        // PROBLEM 4: Compiler optimization issues
        // The compiler might optimize this check away or cache the value
        // in a register, causing the program to miss signal updates
        if (problematic_flag) {
            printf("Main loop detected flag change! Counter: %d\n", problematic_counter);
            
            // PROBLEM 5: Race condition
            // The signal handler might modify these variables
            // while we're reading/writing them
            problematic_flag = 0;
            local_counter++;
            
            // This might show inconsistent values due to race conditions
            printf("Local counter: %d, Global counter: %d\n", 
                   local_counter, problematic_counter);
            
            usleep(100000);
        }
        
        usleep(10000);
    }
    
    printf("Problematic implementation completed (might have issues!)\n");
}

// =============================================================================
// PART 3: Demonstrating compiler optimization issues
// =============================================================================

int optimization_flag = 0;  // Regular int, no volatile

void optimization_signal_handler(int sig) {
    (void)sig;
    optimization_flag = 1;
    
    const char msg[] = "OPTIMIZATION: Signal received\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

void test_optimization_issues() {
    printf("\n=== TESTING COMPILER OPTIMIZATION ISSUES ===\n");
    printf("This might work in debug mode but fail with -O2 optimization!\n");
    
    optimization_flag = 0;
    signal(SIGALRM, optimization_signal_handler);
    
    printf("Setting alarm for 2 seconds...\n");
    alarm(2);
    
    // This loop might be optimized away by the compiler
    // because it doesn't see any way for optimization_flag to change
    // (it doesn't know about the signal handler)
    while (!optimization_flag) {
        // Compiler might optimize this to an infinite loop
        // because it assumes optimization_flag never changes
    }
    
    printf("Flag changed! (This might not print with optimization)\n");
}

// =============================================================================
// PART 4: Demonstrating atomic vs non-atomic operations
// =============================================================================

volatile sig_atomic_t atomic_var = 0;
volatile int non_atomic_var = 0;

void atomic_test_handler(int sig) {
    (void)sig;
    
    // This is guaranteed to be atomic
    atomic_var++;
    
    // This might NOT be atomic on some architectures
    // (especially 64-bit values on 32-bit systems)
    non_atomic_var++;
}

void test_atomic_operations() {
    printf("\n=== TESTING ATOMIC vs NON-ATOMIC OPERATIONS ===\n");
    
    atomic_var = 0;
    non_atomic_var = 0;
    
    signal(SIGALRM, atomic_test_handler);
    
    printf("Sending multiple rapid signals...\n");
    
    // Send multiple signals rapidly
    for (int i = 0; i < 5; i++) {
        kill(getpid(), SIGALRM);
        usleep(1000);
    }
    
    usleep(100000); // Let signals process
    
    printf("Atomic variable: %d\n", (int)atomic_var);
    printf("Non-atomic variable: %d\n", non_atomic_var);
    printf("Both should be equal, but non-atomic might have issues\n");
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main(int argc, char *argv[]) {
    if (argc > 1) {
        // Run specific test
        int test_num = atoi(argv[1]);
        switch (test_num) {
            case 1: test_correct_implementation(); break;
            case 2: test_problematic_implementation(); break;
            case 3: test_optimization_issues(); break;
            case 4: test_atomic_operations(); break;
            default:
                printf("Usage: %s [1|2|3|4]\n", argv[0]);
                printf("1: Correct implementation\n");
                printf("2: Problematic implementation\n");
                printf("3: Optimization issues\n");
                printf("4: Atomic operations\n");
                return 1;
        }
    } else {
        // Run all tests
        printf("DEMONSTRATION: volatile sig_atomic_t vs regular variables\n");
        printf("=======================================================\n");
        
        test_correct_implementation();
        test_problematic_implementation();
        test_optimization_issues();
        test_atomic_operations();
    }
    
    return 0;
}
