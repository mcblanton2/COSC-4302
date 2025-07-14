#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

extern void *count_lines(void *);

int main()
{
    FILE *fh = fopen("data.txt", "r");
    if (!fh)
    {
        perror("Failed to open file");
        return 1;
    }

    pthread_t my_thread;
    int *result;

    if (pthread_create(&my_thread, NULL, count_lines, fh) != 0)
    {
        perror("Thread creation failed");
        fclose(fh);
        return 1;
    }

    if (pthread_join(my_thread, (void **)&result) != 0)
    {
        perror("Thread join failed");
        return 1;
    }

    printf("Number of lines: %d\n", *result);
    free(result); // Free dynamically allocated memory
    return 0;
}
