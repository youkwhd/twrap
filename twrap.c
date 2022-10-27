#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "buf.h"
#include "args.h"

#define ARR_SIZE(arr) sizeof arr / sizeof *arr

size_t str_wordlen(const char *word);

int main(int argc, char **argv)
{
    void *arg_line, *arg_force, *arg_skip;
    arg args[] = {
        {{"l", "line"}, ARG_VALUE, &arg_line},
        {{"f", "force"}, ARG_TOGGLE, &arg_force},
        {{"s", "skip"}, ARG_TOGGLE, &arg_skip},
    };

    args_init(argc, argv, args, ARR_SIZE(args));
    buf_init();
    buf_read();

    const size_t COUNT_ALPHABET_MAX = arg_line ? atoi(arg_line) : 65;
    for (size_t i = 0, count = 0; gbuf->buf[i] != '\0'; i++) {
        bool count_reset = gbuf->buf[i] == '\n' ? true : false;

        if (arg_force || arg_skip)
            if (count >= COUNT_ALPHABET_MAX) {
                if (gbuf->buf[i] != '\n') 
                    putchar('\n');

                count_reset = true;
            }

        if (!arg_skip)
            if (gbuf->buf[i] == ' ' && count + (str_wordlen(&gbuf->buf[i + 1]) + 1) >= COUNT_ALPHABET_MAX) {
                gbuf->buf[i] = '\n';
                count_reset = true;
            }

        putchar(gbuf->buf[i]);
        count_reset ? count = 0 : count++;
    }


    args_free(args, ARR_SIZE(args));
    buf_free();
    return 0;
}

size_t str_wordlen(const char *word)
{
    size_t length = 0;

    for (; *word; word++, length++)
        if (*word == ' ' || *word == '\n')
            break;

    return length;
}
