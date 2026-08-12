#include <ctype.h>
#include <string.h>
#include "lexer.h"

void lexer(const char *input, token_list_t *list) {
    int i = 0;
    while (input[i] != '\0') {
        if (isspace(input[i])) { i++; continue; }

        if (input[i] == '|') {
            token_add(list, TOKEN_PIPE, "|");
            i++;
        } else if (input[i] == '<') {
            token_add(list, TOKEN_INPUT, "<");
            i++;
        } else if (input[i] == '>') {
            if (input[i+1] == '>') {
                token_add(list, TOKEN_APPEND, ">>");
                i += 2;
            } else {
                token_add(list, TOKEN_OUTPUT, ">");
                i++;
            }
        } else if (input[i] == '&') {
            token_add(list, TOKEN_BACKGROUND, "&");
            i++;
        } else if (input[i] == '"') {
            i++;
            char buffer[MAX_TOKEN_LEN];
            int j = 0;
            while (input[i] != '"' && input[i] != '\0') {
                if (j < MAX_TOKEN_LEN - 1) buffer[j++] = input[i];
                i++;
            }
            buffer[j] = '\0';
            token_add(list, TOKEN_WORD, buffer);
            if (input[i] == '"') i++;
        } else {
            char buffer[MAX_TOKEN_LEN];
            int j = 0;
            while (input[i] != '\0' && !isspace(input[i]) &&
                   input[i] != '|' && input[i] != '<' &&
                   input[i] != '>' && input[i] != '&' &&
                   input[i] != '"') {
                if (j < MAX_TOKEN_LEN - 1) buffer[j++] = input[i];
                i++;
            }
            buffer[j] = '\0';
            token_add(list, TOKEN_WORD, buffer);
        }
    }
    token_add(list, TOKEN_END, "");
}
