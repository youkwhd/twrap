#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "twrap_buf.h"

#define ARGSIZ 5

typedef enum twrap_arg_type {
    ARG_VALUE,
    ARG_TOGGLE
} twrap_arg_type;

typedef struct twrap_arg {
    char *valid_args[2];
    twrap_arg_type arg_type;
    void **arg_value_override;
} twrap_arg;

void twrap_args_init(const int argc, char **argv, twrap_arg *args, size_t args_size);
void twrap_args_free(twrap_arg *args, size_t args_size);

void twrap_init(FILE *fp);
void twrap_free();

int main(int argc, char **argv)
{
    void *line = NULL;
    twrap_arg args[ARGSIZ] = {
        {{"l", "line"}, ARG_VALUE, (void **)&line},
    };

    twrap_args_init(argc, argv, args, ARGSIZ);
    twrap_init(stdin);

    for (size_t i = 0, count = 0; twrap_gbuf->buf[i] != '\0'; i++) {
        bool count_reset = false;

        if (twrap_gbuf->buf[i] == '\n') {
            count_reset = true;
        } else if (twrap_gbuf->buf[i] == ' ' && count + (twrap_word_length(&twrap_gbuf->buf[i + 1]) + 1) >= (line != NULL ? (size_t)atoi(line) : COUNT_BREAK)) {
            twrap_gbuf->buf[i] = '\n';
            count_reset = true;
        }

        putchar(twrap_gbuf->buf[i]);
        count_reset ? count = 0 : count++;
    }


    twrap_args_free(args, ARGSIZ);
    twrap_free();
    return 0;
}

void twrap_args_free(twrap_arg *args, size_t args_size) 
{
    for (int i = 0; i < (int)args_size; i++)
         if (args[i].arg_type == ARG_TOGGLE && args[i].arg_value_override != NULL && *args[i].arg_value_override != NULL)
             free(*args[i].arg_value_override);
}

void twrap_args_init(const int argc, char **argv, twrap_arg *args, size_t args_size)
{
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' || (argv[i][0] != '-' && argv[i][1] != '-'))
            continue;

        char *val = argv[i];
        while (*val == '-')
            val++;

        for (int j = 0; j < (int)args_size; j++) {
            for (int k = 0; k < 2; k++) {
                if (args[j].valid_args[k] != NULL && strcmp(val, args[j].valid_args[k]) == 0) {
                    if (args[j].arg_type == ARG_TOGGLE) {
                        bool *ptr = malloc(sizeof *ptr);
                        *ptr = true;
                        *args[j].arg_value_override = (void *)ptr;
                    } else if (args[j].arg_type == ARG_VALUE) {
                        *args[j].arg_value_override = argv[++i];
                    }

                    j = args_size;
                    break;
                }
            }
        }
    }
}

void twrap_init(FILE *fp)
{
    twrap_buf_init();

    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (twrap_gbuf->size > twrap_gbuf->bytes)
            twrap_buf_grow();

        twrap_gbuf->buf[twrap_gbuf->size++] = c;
    }
}

void twrap_free()
{
    twrap_buf_free();
}
