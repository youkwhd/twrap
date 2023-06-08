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

void help(void);

int main(int argc, char **argv)
{
    char *arg_line;
    bool *arg_force, *arg_skip, *arg_help, *arg_debug_args;

    arg args[] = {
        {{"l", "line"}, ARG_VALUE_STR, (void *)&arg_line},                  /* how many ASCII code(s) is permitted in a single line */
        {{"f", "force"}, ARG_VALUE_BOOL, (void *)&arg_force},               /* force single word to break up into new line */
        {{"s", "skip"}, ARG_VALUE_BOOL, (void *)&arg_skip},                 /* skip word proportion, break up into new line if met MAX_CHARS */
        {{"h", "help"}, ARG_VALUE_BOOL, (void *)&arg_help},                 /* shows help message then exits */
        {{"d", "debug-args"}, ARG_VALUE_BOOL, (void *)&arg_debug_args},     /* toggle on debugging for arguments */
    };

    args_init(argc, argv, args, ARR_SIZE(args));

    if (*arg_help) {
        help();
        return 0;
    }

    buf *buf_stdin = buf_init();
    buf_read(buf_stdin);

    const size_t MAX_CHARS = arg_line ? atoi(arg_line) : ARG_LINE_DEFAULT;

    for (size_t i = 0, count = 0; buf_stdin->content[i] != '\0'; i++) {
        bool count_reset = false;

        if (buf_stdin->content[i] == '\n')
            i++, count_reset = true;

        if (*arg_force || *arg_skip)
            if (count >= MAX_CHARS)
                count_reset = true;

        if (!*arg_skip)
            if (buf_stdin->content[i] == ' ' && count + (str_wordlen(&buf_stdin->content[i + 1]) + 1) >= MAX_CHARS)
                count_reset = true;

        if (count_reset) {
            i--, count = 0;
            putchar('\n');
            continue;
        }

        putchar(buf_stdin->content[i]);
        count++;
    }

    if (*(bool *)arg_debug_args)
        putchar('\n'), __args_debug(args, ARR_SIZE(args));

    args_free(args, ARR_SIZE(args));
    buf_free(buf_stdin);
    return 0;
}

void help()
{
    printf("Usage: twrap [OPTION] [FILE]\n");
}
