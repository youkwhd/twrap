#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "buf.h"
#include "str.h"
#include "args.h"

#define ARR_SIZE(arr) sizeof arr / sizeof *arr

#define ARG_LINE_DEFAULT 65

int main(int argc, char **argv)
{
    void *arg_line, *arg_force, *arg_skip, *arg_debug_args;
    arg args[] = {
        {{"l", "line"}, ARG_VALUE_STR, &arg_line},                          /* how many ASCII code(s) is permitted in a single line */
        {{"f", "force"}, ARG_VALUE_BOOL_TOGGLE, &arg_force},                /* force single word to break up into new line */
        {{"s", "skip"}, ARG_VALUE_BOOL_TOGGLE, &arg_skip},                  /* skip word proportion, break up into new line if met COUNT_ALPHABET_MAX */
        {{NULL, "debug-args"}, ARG_VALUE_BOOL_TOGGLE, &arg_debug_args},     /* toggle on debugging for arguments */
    };

    args_init(argc, argv, args, ARR_SIZE(args));

    if (*(bool *)arg_debug_args)
        __args_debug(args, ARR_SIZE(args));

    buf *buf_stdin = buf_init();
    buf_read(buf_stdin);

    /* how many ASCII code(s) is permitted in a single line
     */
    const size_t COUNT_ALPHABET_MAX = arg_line ? atoi(arg_line) : ARG_LINE_DEFAULT;

    for (size_t i = 0, count = 1; buf_stdin->content[i] != '\0'; i++) {
        /* if new line occurs then re-count alphabets
         */
        bool count_reset = buf_stdin->content[i] == '\n' ? true : false;

        if (*(bool *)arg_force || *(bool *)arg_skip)
            if (count >= COUNT_ALPHABET_MAX) {
                if (buf_stdin->content[i] != '\n') 
                    putchar('\n');

                count_reset = true;
            }

        if (!*(bool *)arg_skip)
            if (buf_stdin->content[i] == ' ' && count + (str_wordlen(&buf_stdin->content[i + 1]) + 1) >= COUNT_ALPHABET_MAX) {
                buf_stdin->content[i] = '\n';
                count_reset = true;
            }

        putchar(buf_stdin->content[i]);
        count_reset ? count = 1 : count++;
    }


    args_free(args, ARR_SIZE(args));
    buf_free(buf_stdin);
    return 0;
}
