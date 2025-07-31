/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

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
	pid_t pid;
	char buffer[BUFFER_SIZE];
	int fd[2];

	if(pipe(fd) ==-1) {
		perror("pipe failed");
		return 1;
	}

	pid = fork();

	if(pid < 0) {
		perror("fork failed");
		return 1;
	}

	if(pid == 0) {
		//child process
		close(fd[1]);
		ssize_t n;
		while((n = read(fd[0], buffer, BUFFER_SIZE)) >0) {
			for(ssize_t i = 0; i < n; i++) {
				if(isupper(buffer[i]))
					buffer[i] = tolower(buffer[i]);

				else if(islower(buffer[i]))
					buffer[i] = toupper(buffer[i]);
			}
			write(STDOUT_FILENO, buffer, n);
		}
		close(fd[0]);
		return 1;
	}
	else {
		// parent process
		close(fd[0]);

		// open input file
		FILE *fp = fopen("input.txt", "r");
		if(!fp) {
			perror("fopen");
			close(fd[1]);
			return 1;
		}
		size_t n;
		while((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
			write(fd[1], buffer, n);
		}

		fclose(fp);
		close(fd[1]);

		wait(NULL);
	}

		return 0;
	}