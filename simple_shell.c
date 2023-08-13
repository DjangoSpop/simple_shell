#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream)
{
    ssize_t pos = 0;
    int c;

    if (*lineptr == NULL)
    {
        *lineptr = malloc(*n);
        if (*lineptr == NULL)
            return -1;
    }

    while ((c = fgetc(stream)) != EOF && c != '\n')
    {
        if (pos >= *n - 1)
        {
            *n *= 2;
            *lineptr = realloc(*lineptr, *n);
            if (*lineptr == NULL)
                return -1;
        }
        (*lineptr)[pos++] = c;
    }

    if (c == EOF && pos == 0)
        return -1;

    (*lineptr)[pos] = '\0';
    return pos;
}

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        printf("$ ");
        read = my_getline(&line, &len, stdin);

        if (read == -1)
        {
            perror("getline");
            break;
        }

        // Remove newline character if present
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        // Tokenize input
        char *token;
        char *args[10]; // Adjust as needed
        int i = 0;

        token = strtok(line, " ");
        while (token != NULL)
        {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;

        // Built-in commands
        if (strcmp(args[0], "exit") == 0)
        {
            free(line);
            exit(EXIT_SUCCESS); // Task 4
        }
        else if (strcmp(args[0], "env") == 0)
        {
            char **env_ptr = env;
            while (*env_ptr)
            {
                printf("%s\n", *env_ptr);
                env_ptr++;
            }
            continue; // Task 5
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            if (i == 1) // Only "cd" command
            {
                chdir(getenv("HOME")); // Change to home directory
            }
            else if (i == 2) // "cd" with an argument
            {
                chdir(args[1]); // Change to specified directory
            }
            continue;
        }

        // Fork and execute command
        pid_t pid = fork();
        if (pid == 0)
        {
            // Handle the PATH environment variable
            char *path = getenv("PATH");
            char *path_copy = strdup(path);
            char *path_token = strtok(path_copy, ":");
            char *full_path = NULL;

            while (path_token != NULL)
            {
                full_path = malloc(strlen(path_token) + strlen(args[0]) + 2);
                strcpy(full_path, path_token);
                strcat(full_path, "/");
                strcat(full_path, args[0]);

                execve(full_path, args, env);
                free(full_path);
                path_token = strtok(NULL, ":");
            }

            perror("execve");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
        }
    }

    free(line);
    return (0);
}
