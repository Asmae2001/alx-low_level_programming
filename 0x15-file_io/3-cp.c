#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

char *create_buffer(void);
void close_file(int fd);

char *create_buffer(void)
{
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);

    if (buffer == NULL)
    {
        perror("Error: Unable to allocate buffer");
        exit(EXIT_FAILURE);
    }

    return buffer;
}

void close_file(int fd)
{
    if (close(fd) == -1)
    {
        perror("Error: Unable to close file descriptor");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int from, to, r, w;
    char *buffer;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s file_from file_to\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    buffer = create_buffer();
    from = open(argv[1], O_RDONLY);
    
    if (from == -1)
    {
        perror("Error: Unable to open source file");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    to = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
    if (to == -1)
    {
        perror("Error: Unable to open destination file");
        free(buffer);
        close_file(from);
        exit(EXIT_FAILURE);
    }

    do
    {
        r = read(from, buffer, BUFFER_SIZE);
        
        if (r == -1)
        {
            perror("Error: Unable to read from source file");
            free(buffer);
            close_file(from);
            close_file(to);
            exit(EXIT_FAILURE);
        }

        w = write(to, buffer, r);
        
        if (w == -1 || w != r)
        {
            perror("Error: Unable to write to destination file");
            free(buffer);
            close_file(from);
            close_file(to);
            exit(EXIT_FAILURE);
        }

    } while (r > 0);

    free(buffer);
    close_file(from);
    close_file(to);

    return 0;
}

