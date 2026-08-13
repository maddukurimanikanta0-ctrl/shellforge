#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "lexer.h"
#include "token.h"

static void print_banner(void)
{
    printf("====================================\n");
    printf("          Shellforge\n");
    printf("   A Unix Style Shell written in C\n");
    printf("====================================\n");
}

static void print_command_history(void)
{
    HIST_ENTRY **history;
    int i;

    history = history_list();

    printf("\n");
    printf("------ Command History ------\n");

    if (history != NULL) {
        for (i = 0; history[i] != NULL; i++) {
            printf("%d  %s\n",
                   i + 1,
                   history[i]->line);
        }
    }

    printf("-----------------------------\n");
}

int main(void)
{
    char *input;
    token_list_t list;

    print_banner();

    while (1) {

        input = readline("shellforge$ ");

        /* Ctrl+D */
        if (input == NULL) {
            printf("\nExiting...\n");
            break;
        }

        /* Ignore empty input */
        if (strlen(input) == 0) {
            free(input);
            continue;
        }

        /*
         * Store command in history.
         */
        add_history(input);

        /*
         * Exit command.
         */
        if (strcmp(input, "exit") == 0) {
            free(input);
            printf("Exiting...\n");
            break;
        }

        /*
         * History command.
         */
        if (strcmp(input, "history") == 0) {
            print_command_history();
            free(input);
            continue;
        }

        /*
         * Tokenize command.
         */
        lexer(input, &list);

        /*
         * Print generated tokens.
         */
        token_print(&list);

        free(input);
    }

    return 0;
}
