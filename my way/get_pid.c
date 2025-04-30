#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

void first_technique(int my_pid)
{
    printf("Process ID: %d\n", my_pid);
    printf("Process ID: %d \n", getpid());
}

void signal_handler(int signum) {
    printf("\e[34mCaught signal %i! Not terminating.\e[0m\n", signum);
    // No exit() here means the program continues
}

void usr1_handler(int signum)
{
    printf("\e[32mCustom action for SIGURS1\e[0m\n");
}

void usr2_handler(int signum)
{
    printf("\e[31mCustom action for SIGUSR2\e[0m\n");
}

void second_technique(int my_pid)
{
    printf("my PID accordign to getpid() is %d\n", my_pid);
    char cmd[100];
    sprintf(cmd, "ps -p %d -o pid,cmd", my_pid);
    system(cmd);
    printf("\n\n");
}

void third_technique(void)
{
    printf("PID at start: %d\n", getpid());
    sleep(2);
    printf("PID after sleep: %d\n", getpid());
    sleep(2);
    printf("PID after sleep: %d\n", getpid());
}

int main(void)
{
    pid_t pid = getpid();
    signal(SIGUSR1, usr1_handler);
    signal(SIGUSR2, usr2_handler);
    first_technique(pid);
    second_technique(pid);
    kill(pid, SIGUSR1);
    kill(pid, SIGUSR2);
    kill(pid, SIGTERM);
    third_technique();
    return (0);
}