#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    printf("Before fork\n");
    pid = fork(); // Create a new process

    if (pid < 0)
    {
        // Fork failed
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child process: PID = %d\n", getpid());
        printf("Hello from the child process!\n");
    }
    else
    {
        // Parent process
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);
        wait(NULL); // Wait for child process to finish
        printf("Child process finished.\n");
    }
}