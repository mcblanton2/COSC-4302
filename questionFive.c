#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // for fork()
#include <sys/wait.h> // for wait()

int main()
{
    pid_t pid = fork(); // Create a child process

    if (pid < 0)
    {
        // Fork failed
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child Process: Sum of odd numbers from 1 to 1000
        int sum_odd = 0;
        for (int i = 1; i <= 1000; i++)
        {
            if (i % 2 != 0)
            {
                sum_odd += i;
            }
        }
        printf("Child Process (PID: %d) - Sum of odd numbers = %d\n", getpid(), sum_odd);
        exit(0); // Child process terminates
    }
    else
    {
        // Parent Process: Sum of even numbers from 1 to 1000
        int sum_even = 0;
        for (int i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
            {
                sum_even += i;
            }
        }
        printf("Parent Process (PID: %d) - Sum of even numbers = %d\n", getpid(), sum_even);

        // Wait for child process to terminate
        wait(NULL);
        printf("Parent Process - Child process terminated. Parent terminating.\n");
    }

    return 0;
}
