#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "twrap_buf.h"
#include "twrap_args.h"

#define ARGSIZ 5

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
