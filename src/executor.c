#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "executor.h"

int execute_pipeline(pipeline_t *pipeline)
{
    int i;
    int prev_fd = -1;
    int pipefd[2];
    pid_t pid;
    int status;

    if (pipeline == NULL)
        return -1;

    for (i = 0; i < pipeline->command_count; i++)
    {
        command_t *cmd = &pipeline->commands[i];

        if (cmd->argv[0] == NULL)
            continue;

        /* Create pipe except for the last command */
        if (i < pipeline->command_count - 1)
        {
            if (pipe(pipefd) < 0)
            {
                perror("pipe");
                return -1;
            }
        }

        pid = fork();

        if (pid < 0)
        {
            perror("fork");
            return -1;
        }

        if (pid == 0)
        {
            int fd;

            /* Input from previous command */
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            /* Output to next command */
            if (i < pipeline->command_count - 1)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            /* Input redirection */
            if (cmd->input_file != NULL)
            {
                fd = open(cmd->input_file, O_RDONLY);

                if (fd < 0)
                {
                    perror(cmd->input_file);
                    exit(EXIT_FAILURE);
                }

                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            /* Output redirection */
            if (cmd->output_file != NULL)
            {
                if (cmd->append)
                {
                    fd = open(cmd->output_file,
                              O_WRONLY | O_CREAT | O_APPEND,
                              0644);
                }
                else
                {
                    fd = open(cmd->output_file,
                              O_WRONLY | O_CREAT | O_TRUNC,
                              0644);
                }

                if (fd < 0)
                {
                    perror(cmd->output_file);
                    exit(EXIT_FAILURE);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            execvp(cmd->argv[0], cmd->argv);

            perror("execvp");
            exit(EXIT_FAILURE);
        }

        /* Parent */

        if (prev_fd != -1)
            close(prev_fd);

        if (i < pipeline->command_count - 1)
        {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }
    }

    if (prev_fd != -1)
        close(prev_fd);

    for (i = 0; i < pipeline->command_count; i++)
    {
        wait(&status);
    }

    return 0;
}
