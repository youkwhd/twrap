#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "twrap_buf.h"
#include "twrap_args.h"

#define ARR_SIZE(arr) sizeof arr / sizeof *arr

size_t twrap_str_wordlen(const char *word);
void twrap_init(FILE *fp);
void twrap_free();

int main(int argc, char **argv)
{
    void *arg_line = NULL, *arg_force = NULL, *arg_skip;
    twrap_arg args[] = {
        {{"l", "line"}, ARG_VALUE, (void **)&arg_line},
        /* if there's one single long word longer than expected arg_line 
         * then it forces the word to break (open new line)
         */
        {{"f", "force"}, ARG_TOGGLE, (void **)&arg_force},
        {{"s", "skip"}, ARG_TOGGLE, (void **)&arg_skip},
    };

    twrap_args_init(argc, argv, args, ARR_SIZE(args));
    twrap_init(stdin);

    const size_t COUNT_ALPHABET_MAX = arg_line ? atoi(arg_line) : 65;
    for (size_t i = 0, count = 0; twrap_gbuf->buf[i] != '\0'; i++) {
        bool count_reset = twrap_gbuf->buf[i] == '\n' ? true : false;

        if (arg_force || arg_skip)
            if (count >= COUNT_ALPHABET_MAX) {
                if (twrap_gbuf->buf[i] != '\n') 
                    putchar('\n');

                count_reset = true;
            }

        if (!arg_skip)
            if (twrap_gbuf->buf[i] == ' ' && count + (twrap_str_wordlen(&twrap_gbuf->buf[i + 1]) + 1) >= COUNT_ALPHABET_MAX) {
                twrap_gbuf->buf[i] = '\n';
                count_reset = true;
            }

        putchar(twrap_gbuf->buf[i]);
        count_reset ? count = 0 : count++;
    }

    twrap_args_free(args, ARR_SIZE(args));
    twrap_free();
    return 0;
}

size_t twrap_str_wordlen(const char *word)
{
    size_t length = 0;

    for (; *word; word++, length++)
        if (*word == ' ' || *word == '\n')
            break;

    return length;
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
