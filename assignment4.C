#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main()
{

    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // child filter
        close(pipefd[1]); // close write & end of pipe.

        ssize_t n;
        while ((n = read(pipefd[0], buffer, BUFFER_SIZE)) > 0)
        {
            for (ssize_t i = 0; i < n; i++)
            {
                if (isupper(buffer[i]))
                    buffer[i] = tolower(buffer[i]);
                else if (islower(buffer[i]))
                    buffer[i] = toupper(buffer[i]);
            }
            write(STDOUT_FILENO, buffer, n);
        }

        close(pipefd[0]); // close the read end
        exit(EXIT_SUCCESS);
    }
    else
    {
        // parent process
        close(pipefd[0]);

        // open input file
        FILE *fp = fopen("input.txt", "r");
        if (!fp)
        {
            perror("fopen");
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }

        size_t n;
        while ((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0)
        {
            write(pipefd[1], buffer, n);
        }

        fclose(fp);
        close(pipefd[1]); // signals end of file or EOD

        wait(NULL);
    }

    return 0;
}
