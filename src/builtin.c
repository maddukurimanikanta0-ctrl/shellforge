#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "builtin.h"

int is_builtin(command_t *cmd)
{
    if (cmd == NULL || cmd->argv[0] == NULL)
        return 0;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "pwd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "echo") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "exit") == 0)
        return 1;

    return 0;
}

static int builtin_cd(command_t *cmd)
{
    char *dir;

    if (cmd->argv[1] == NULL)
        dir = getenv("HOME");
    else
        dir = cmd->argv[1];

    if (cmd->argv[2] != NULL)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    if (dir == NULL || chdir(dir) != 0)
    {
        perror("cd");
        return 1;
    }

    return 0;
}

static int builtin_pwd(command_t *cmd)
{
    char buffer[1024];

    if (cmd->argv[1] != NULL)
    {
        fprintf(stderr, "pwd: too many arguments\n");
        return 1;
    }

    if (getcwd(buffer, sizeof(buffer)) == NULL)
    {
        perror("pwd");
        return 1;
    }

    printf("%s\n", buffer);

    return 0;
}

static int builtin_echo(command_t *cmd)
{
    int i;

    for (i = 1; cmd->argv[i] != NULL; i++)
    {
        printf("%s", cmd->argv[i]);

        if (cmd->argv[i + 1] != NULL)
            printf(" ");
    }

    printf("\n");

    return 0;
}

int execute_builtin(command_t *cmd)
{
    if (strcmp(cmd->argv[0], "cd") == 0)
        return builtin_cd(cmd);

    if (strcmp(cmd->argv[0], "pwd") == 0)
        return builtin_pwd(cmd);

    if (strcmp(cmd->argv[0], "echo") == 0)
        return builtin_echo(cmd);

    if (strcmp(cmd->argv[0], "exit") == 0)
        return 1;

    return 0;
}
