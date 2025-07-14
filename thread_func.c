#include <stdio.h>
#include <stdlib.h>

void *count_lines(void *arg)
{
    FILE *fh = (FILE *)arg;
    int *num_lines = malloc(sizeof(int));
    if (!num_lines)
        return NULL;

    *num_lines = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), fh))
    {
        (*num_lines)++;
    }

    fclose(fh);
    return (void *)num_lines;
}
