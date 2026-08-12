#include <stdio.h>
#include <string.h>
#include "token.h"
#include "lexer.h"

int main() {
    char input[256];
    token_list_t list;

    printf("===============================\n");
    printf("Shellforge\n");
    printf("A Unix Style Shell written in C\n");
    printf("===============================\n");

    while (1) {
        printf("shellforge$ ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting...\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        // Exit command
        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        token_list_init(&list);
        lexer(input, &list);
        token_print(&list);
    }

    return 0;
}
