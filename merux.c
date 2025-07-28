#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <linux/limits.h>

#define MAX_LINE 1024 
#define MAX_ARGS 64

// Search for command in $PATH
char* find_fullpath(const char* cmd) {
    char *path_env = getenv("PATH");
    static char fullpath[PATH_MAX];

    // Create a modifable copy of PATH
    char path_copy[PATH_MAX];
    strncpy(path_copy, path_env, PATH_MAX);
    path_copy[PATH_MAX -1];

    char *path = strtok(path_copy, ":");
    while (path != NULL) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, cmd);
        if (access(fullpath, X_OK) == 0) {
            return fullpath;
        }
        path = strtok(NULL, ":");
    }

    return NULL; // not found
}

// Put input line into arguments
void parse_command(char *line, char **argv) {
    int i = 0;
    argv[i] = strtok(line, " \t\n");
    while (argv[i] != NULL && i < MAX_ARGS - 1) {
        argv[++i] = strtok(NULL, " \t\n");
    }
    argv[i] = NULL;
}

int main() {
    char line[MAX_LINE];
    char *argv[MAX_ARGS];
    // PROMPTS THE USER FOR COMMAND
    printf("Enter Command \n");

    while (1) {
        printf("mrx> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break; 
        }

        // Skip empty input
        if (line[0] == '\n') continue;

        // PARSE THE COMMAND 
        parse_command(line, argv);
        if (argv[0] == NULL) continue;

        // Special case: exit
        if (strcmp(argv[0], "exit") == 0) break;

        // Find the full path of the command using $PATH
        char *fullpath = find_fullpath(argv[0]);
        if (fullpath == NULL) {
            fprintf(stderr, "Command not found: %s\n", argv[0]);
            continue;
        }

        // FORK AND EXECUTE COMMAND 
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            // Child
            printf("Executing: %s\n", fullpath);
            execv(fullpath, argv);
            perror("execv failed");
            exit(1);
        } else {
            // Parent
            wait(NULL);
        }
    }

    printf("Exiting shell. \n");
    return 0;
}
