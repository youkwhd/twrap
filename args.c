#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "args.h"
#include "str.h"

arg *args_find_arg(arg *args, size_t args_size, const char *arg_flag, arg_type arg_type)
{
    for (size_t i = 0; i < args_size; i++)
        if (args[i].valid_args[arg_type] != NULL && strcmp(arg_flag, args[i].valid_args[arg_type]) == 0)
            return args + i;

    return NULL;
}


void args_parse_long(const int argc, char **argv, arg *args, size_t args_size)
{
    const size_t ARG_DASH_COUNT = 2;

    for (int i = 1; i < argc; i++) {
        if (str_firstch(argv[i], '-') != ARG_DASH_COUNT)
            continue;

        char *val = argv[i] + ARG_DASH_COUNT;
        arg *arg_target = args_find_arg_long(args, args_size, val);

        switch (arg_target->arg_value_type) {
        case ARG_VALUE_BOOL_TOGGLE:
            /* prevents memory leaks
             */
            if (*arg_target->arg_value_ptr != NULL)
                break;

            bool *tmp_ptr = malloc(sizeof *tmp_ptr);
            *tmp_ptr = true;
            *arg_target->arg_value_ptr = (void *)tmp_ptr;
            break;
        case ARG_VALUE_STR:
            *arg_target->arg_value_ptr = argv[++i];
            break;
        }
    }
}

void args_parse_short(const int argc, char **argv, arg *args, size_t args_size)
{
    const size_t ARG_DASH_COUNT = 1;

    for (int i = 1; i < argc; i++) {
        if (str_firstch(argv[i], '-') != ARG_DASH_COUNT)
            continue;

        char *val = argv[i] + ARG_DASH_COUNT;
        size_t val_len = strlen(val), val_it = 0;
        bool is_multiple_toggle = false;

        /* since we are parsing
         * short arguments with multiple flags in a single dash,
         * cut and parse it character-by-character.
         */
        while (val_it < val_len) {
            const char flag[2] = {val[val_it], '\0'};
            arg *arg_target = args_find_arg_short(args, args_size, flag);

            switch (arg_target->arg_value_type) {
            case ARG_VALUE_BOOL_TOGGLE:
                /* prevents memory leaks
                 */
                if (*arg_target->arg_value_ptr != NULL)
                    break;

                bool *tmp_ptr = malloc(sizeof *tmp_ptr);
                *tmp_ptr = true, is_multiple_toggle = true;
                *arg_target->arg_value_ptr = (void *)tmp_ptr;
                break;
            case ARG_VALUE_STR:
                /* possible arg that has value 
                 * misplaced with multiple toggle args
                 *
                 * this only occurs on single dash arguments
                 */
                if (is_multiple_toggle)
                    break;

                *arg_target->arg_value_ptr = argv[++i];
                break;
            }

            val_it++;
        }
    }
}

void args_init(const int argc, char **argv, arg *args, size_t args_size)
{
    for (size_t i = 0; i < args_size; i++)
        *args[i].arg_value_ptr = NULL;

    args_parse_short(argc, argv, args, args_size);
    args_parse_long(argc, argv, args, args_size);
}

void args_free(arg *args, size_t args_size) 
{
    for (size_t i = 0; i < args_size; i++)
         if (args[i].arg_value_type == ARG_VALUE_BOOL_TOGGLE && *args[i].arg_value_ptr != NULL)
             free(*args[i].arg_value_ptr);
}

void __args_debug(arg *args, size_t args_size)
{
    for (size_t i = 0; i < args_size; i++) {
        if (*args[i].arg_value_ptr) {
            printf("[ARGS_DEBUG] %s -> ", *args[i].valid_args);

            switch (args[i].arg_value_type) {
            case ARG_VALUE_STR:
                puts((char *)*args[i].arg_value_ptr);
                break;
            case ARG_VALUE_BOOL_TOGGLE:
                puts(*((bool *)*args[i].arg_value_ptr) ? "<true>" : "<false>");
                break;
            }
        }
    }
}
