#include <stdlib.h>
#include <string.h>
#include "expand.h"

static void expand_text(char *text)
{
    char result[1024];
    char name[128];

    char *dollar;
    char *p;

    const char *value;

    int i;
    int j;

    if (text == NULL)
        return;

    dollar = strchr(text, '$');

    if (dollar == NULL)
        return;

    i = 0;

    while (text < dollar && i < 1023)
        result[i++] = *text++;

    p = dollar + 1;

    j = 0;

    while ((*p >= 'A' && *p <= 'Z') ||
           (*p >= 'a' && *p <= 'z') ||
           (*p >= '0' && *p <= '9') ||
           *p == '_')
    {
        if (j < 127)
            name[j++] = *p;

        p++;
    }

    name[j] = '\0';

    value = getenv(name);

    if (value == NULL)
        value = "";

    while (*value && i < 1023)
        result[i++] = *value++;

    while (*p && i < 1023)
        result[i++] = *p++;

    result[i] = '\0';

    strcpy(text, result);
}

void expand_variables(pipeline_t *pipeline)
{
    int i;
    int j;

    if (pipeline == NULL)
        return;

    for (i = 0; i < pipeline->command_count; i++)
    {
        command_t *cmd =
            &pipeline->commands[i];

        for (j = 0; cmd->argv[j] != NULL; j++)
            expand_text(cmd->argv[j]);

        if (cmd->input_file != NULL)
            expand_text(cmd->input_file);

        if (cmd->output_file != NULL)
            expand_text(cmd->output_file);
    }
}
