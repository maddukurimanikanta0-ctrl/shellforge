#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"

void lexer(const char *input, token_list_t *list)
{
    int i = 0;

    if (input == NULL || list == NULL) {
        return;
    }

    token_list_init(list);

    while (input[i] != '\0') {

        /* Skip whitespace */
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        /* Pipe */
        if (input[i] == '|') {
            token_add(list, TOKEN_PIPE, "|");
            i++;
            continue;
        }

        /* Input redirection */
        if (input[i] == '<') {
            token_add(list, TOKEN_INPUT, "<");
            i++;
            continue;
        }

        /* Output redirection / Append */
        if (input[i] == '>') {
            if (input[i + 1] == '>') {
                token_add(list, TOKEN_APPEND, ">>");
                i += 2;
            } else {
                token_add(list, TOKEN_OUTPUT, ">");
                i++;
            }
            continue;
        }

        /* Background */
        if (input[i] == '&') {
            token_add(list, TOKEN_BACKGROUND, "&");
            i++;
            continue;
        }

        /* Build WORD token */
        {
            char word[MAX_TOKEN_LEN];
            int j = 0;
            char c;

            while (input[i] != '\0') {

                c = input[i];

                /* Delimiters */
                if (isspace((unsigned char)c) ||
                    c == '|' ||
                    c == '<' ||
                    c == '>' ||
                    c == '&') {
                    break;
                }

                /* Single quote */
                if (c == '\'') {
                    i++;

                    while (input[i] != '\0' && input[i] != '\'') {
                        if (j < MAX_TOKEN_LEN - 1) {
                            word[j++] = input[i];
                        }
                        i++;
                    }

                    if (input[i] == '\0') {
                        printf("Lexer Error: Unterminated single quote\n");
                        return;
                    }

                    i++;
                    continue;
                }

                /* Double quote */
                if (c == '"') {
                    i++;

                    while (input[i] != '\0' && input[i] != '"') {

                        /* Escaped character */
                        if (input[i] == '\\') {
                            i++;

                            if (input[i] == '\0') {
                                printf("Lexer Error: Unterminated double quote\n");
                                return;
                            }

                            if (j < MAX_TOKEN_LEN - 1) {
                                word[j++] = input[i];
                            }

                            i++;
                            continue;
                        }

                        if (j < MAX_TOKEN_LEN - 1) {
                            word[j++] = input[i];
                        }

                        i++;
                    }

                    if (input[i] == '\0') {
                        printf("Lexer Error: Unterminated double quote\n");
                        return;
                    }

                    i++;
                    continue;
                }

                /* Escape character */
                if (c == '\\') {
                    i++;

                    if (input[i] == '\0') {
                        break;
                    }

                    if (j < MAX_TOKEN_LEN - 1) {
                        word[j++] = input[i];
                    }

                    i++;
                    continue;
                }

                /* Normal character */
                if (j < MAX_TOKEN_LEN - 1) {
                    word[j++] = c;
                }

                i++;
            }

            word[j] = '\0';

            if (j > 0) {
                token_add(list, TOKEN_WORD, word);
            }
        }
    }

    /* Add END token */
    token_add(list, TOKEN_END, "END");
}
