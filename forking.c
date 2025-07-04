#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    printf("Parent: Hello! I'm the parent process.\n");

    pid = fork(); // Create child process

    if (pid < 0)
    {
        // Fork failed
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child: Hello! I'm the child process. My PID is %d\n", getpid());
        sleep(20);
        printf("Child: I'm done sleeping, exiting now.\n");
        exit(0);
    }
    else
    {
        // Parent process
        int status;
        printf("Parent: I created a child process with PID %d\n", pid);

        // Wait for child to finish
        waitpid(pid, &status, 0);

        printf("Parent: My child process has terminated.\n");
        printf("Parent: Exiting now.\n");
    }

    return 0;
}
