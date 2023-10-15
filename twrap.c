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

void print_usage_and_exit(char *program_name, int exit_code)
{
    printf("Usage: %s [OPTION ...]\n"
           "Text wrapper, wraps text from stdin into stdout.\n"
           "\n"
           "Options:"
           "    -l, --line NUMBER       How many character in a single line.\n"
           "    -f, --force             Forces single word to keep on the line\n"
           "                            useful for keeping URLs proportion.\n"
           "    -s, --skip              Skips proportion, break line when exceeded.\n"
           "    -h, --help              Prints this message and exit.\n", program_name);
    exit(exit_code);
}

/* TODO: accept file other than stdin
 */
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

    if (*arg_help)
        print_usage_and_exit(argv[0], 0);

    if (arg_line && atoi(arg_line) <= 0) {
        fprintf(stderr, "%s: option -l | --line must be more than `%s`.\n", argv[0], arg_line);
        return 1;
    }

    buf *buf_stdin = buf_init();
    buf_read(buf_stdin);

    const size_t MAX_CHARS = arg_line ? atoi(arg_line) : ARG_LINE_DEFAULT;

    for (size_t i = 0, count = 0; buf_stdin->content[i] != '\0'; i++, count++) {
        bool count_reset = false;

        if (buf_stdin->content[i] == '\n') {
            count_reset = true;
            i++;
        }

        if (!*arg_skip) {
            if (buf_stdin->content[i] == ' ' && (count + 1) + (str_wordlen(&buf_stdin->content[i + 1])) > MAX_CHARS) {
                count_reset = true;
                i++;
            }
        }

        if (!*arg_force || *arg_skip) {
            if (count >= MAX_CHARS) {
                count_reset = true;
            }
        }

        if (count_reset) {
            count = 0;
            i--;
            putchar('\n');
            continue;
        }

        putchar(buf_stdin->content[i]);
    }

    if (*arg_debug_args) {
        putchar('\n');
        __args_debug(args, ARR_SIZE(args));
    }

    args_free(args, ARR_SIZE(args));
    buf_free(buf_stdin);
    return 0;
}

