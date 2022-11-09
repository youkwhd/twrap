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
        {{"l", "line"}, ARG_VALUE, &arg_line},      /* how many ASCII code(s) is permitted in a single line */
        {{"f", "force"}, ARG_TOGGLE, &arg_force},   /* force single word to break up into new line */
        {{"s", "skip"}, ARG_TOGGLE, &arg_skip},     /* skip word proportion, break up into new line if met COUNT_ALPHABET_MAX */
    };

    args_init(argc, argv, args, ARR_SIZE(args));

    buf *buf_stdin = buf_init();
    buf_read(buf_stdin);

    /* how many ASCII code(s) is permitted in a single line
     */
    const size_t COUNT_ALPHABET_MAX = arg_line ? atoi(arg_line) : 65;

    for (size_t i = 0, count = 1; buf_stdin->buf[i] != '\0'; i++) {
        /* if new line occurs then re-count alphabets
         */
        bool count_reset = buf_stdin->buf[i] == '\n' ? true : false;

        if (arg_force || arg_skip)
            if (count >= COUNT_ALPHABET_MAX) {
                if (buf_stdin->buf[i] != '\n') 
                    putchar('\n');

                count_reset = true;
            }

        if (!arg_skip)
            if (buf_stdin->buf[i] == ' ' && count + (str_wordlen(&buf_stdin->buf[i + 1]) + 1) >= COUNT_ALPHABET_MAX) {
                buf_stdin->buf[i] = '\n';
                count_reset = true;
            }

        putchar(buf_stdin->buf[i]);
        count_reset ? count = 1 : count++;
    }


    args_free(args, ARR_SIZE(args));
    buf_free(buf_stdin);
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
