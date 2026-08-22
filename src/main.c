#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "expand.h"
#include "builtin.h"
#include "executor.h"

int main(void)
{
    char *line = NULL;
    size_t size = 0;

    token_list_t tokens;
    pipeline_t pipeline;

    printf("====================================\n");
    printf("             ShellForge\n");
    printf("====================================\n");

    while (1)
    {
        printf("shellforge$ ");
        fflush(stdout);

        if (getline(&line, &size, stdin) == -1)
            break;

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0)
            continue;

        lexer(line, &tokens);

        token_print(&tokens);

        if (parser(&tokens, &pipeline))
        {
            expand_variables(&pipeline);

            pipeline_print(&pipeline);

            /*
             * Built-in commands must run inside the
             * shell process so that cd changes the
             * shell's directory.
             *
             * If a built-in has redirection, execute
             * it through the executor so that >, >>,
             * and < are handled.
             */
            if (pipeline.command_count == 1 &&
                is_builtin(&pipeline.commands[0]))
            {
                command_t *cmd = &pipeline.commands[0];

                if (cmd->input_file != NULL ||
                    cmd->output_file != NULL)
                {
                    execute_pipeline(&pipeline);
                }
                else
                {
                    if (execute_builtin(cmd))
                        break;
                }
            }
            else
            {
                execute_pipeline(&pipeline);
            }
        }

        free(line);
        line = NULL;
        size = 0;
    }

    free(line);

    return 0;
}
