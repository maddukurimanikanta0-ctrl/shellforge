#include <stdio.h>
#include "parser.h"

static void command_init(command_t *cmd)
{
    int i;

    for (i = 0; i < MAX_ARGS; i++)
        cmd->argv[i] = NULL;

    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = 0;
    cmd->background = 0;
}

int parser(token_list_t *tokens, pipeline_t *pipeline)
{
    int i;
    int argc = 0;
    command_t *cmd;

    if (tokens == NULL || pipeline == NULL)
        return 0;

    pipeline->command_count = 1;

    cmd = &pipeline->commands[0];
    command_init(cmd);

    for (i = 0; i < tokens->count; i++)
    {
        token_t *t = &tokens->tokens[i];

        if (t->type == TOKEN_END)
            break;

        if (t->type == TOKEN_PIPE)
        {
            cmd->argv[argc] = NULL;

            pipeline->command_count++;

            if (pipeline->command_count > MAX_COMMANDS)
                return 0;

            cmd = &pipeline->commands[
                pipeline->command_count - 1
            ];

            command_init(cmd);
            argc = 0;
        }
        else if (t->type == TOKEN_INPUT)
        {
            if (i + 1 >= tokens->count)
                return 0;

            cmd->input_file =
                tokens->tokens[++i].text;
        }
        else if (t->type == TOKEN_OUTPUT)
        {
            if (i + 1 >= tokens->count)
                return 0;

            cmd->output_file =
                tokens->tokens[++i].text;

            cmd->append = 0;
        }
        else if (t->type == TOKEN_APPEND)
        {
            if (i + 1 >= tokens->count)
                return 0;

            cmd->output_file =
                tokens->tokens[++i].text;

            cmd->append = 1;
        }
        else if (t->type == TOKEN_BACKGROUND)
        {
            cmd->background = 1;
        }
        else if (t->type == TOKEN_WORD)
        {
            if (argc >= MAX_ARGS - 1)
                return 0;

            cmd->argv[argc++] = t->text;
        }
    }

    cmd->argv[argc] = NULL;

    return 1;
}

void pipeline_print(const pipeline_t *pipeline)
{
    int i;
    int j;

    if (pipeline == NULL)
        return;

    printf("\n========== PIPELINE ==========\n");

    for (i = 0; i < pipeline->command_count; i++)
    {
        command_t *cmd =
            (command_t *)&pipeline->commands[i];

        printf("\nCommand %d\n", i + 1);
        printf("------------------------------\n");

        printf("Arguments\n");

        for (j = 0; cmd->argv[j] != NULL; j++)
            printf("argv[%d] = %s\n",
                   j,
                   cmd->argv[j]);

        printf("Input      : %s\n",
               cmd->input_file ?
               cmd->input_file : "None");

        printf("Output     : %s\n",
               cmd->output_file ?
               cmd->output_file : "None");

        printf("Append     : %s\n",
               cmd->append ? "Yes" : "No");

        printf("Background : %s\n",
               cmd->background ? "Yes" : "No");
    }

    printf("==============================\n");
}
