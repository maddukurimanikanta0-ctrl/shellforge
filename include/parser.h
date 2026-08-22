#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_COMMANDS 20
#define MAX_ARGS 50

typedef struct
{
    char *argv[MAX_ARGS];

    char *input_file;
    char *output_file;

    int append;
    int background;
} command_t;

typedef struct
{
    command_t commands[MAX_COMMANDS];
    int command_count;
} pipeline_t;

int parser(token_list_t *tokens, pipeline_t *pipeline);

void pipeline_print(const pipeline_t *pipeline);

#endif
