#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "args.h"
#include "str.h"

#define ARR_SIZE(arr) sizeof arr / sizeof *arr

arg *args_find_arg(arg *args, size_t args_size, const char *arg_flag, arg_type arg_type)
{
    for (size_t i = 0; i < args_size; i++)
        if (args[i].valid_args[arg_type] != NULL && strcmp(arg_flag, args[i].valid_args[arg_type]) == 0)
            return args + i;

    return NULL;
}


void args_parse_long(const int argc, char **argv, arg *args, size_t args_size)
{
    const size_t ARG_MIN_DASH = 2;

    for (int i = 1; i < argc; i++) {
        if (str_firstch(argv[i], '-') != ARG_MIN_DASH)
            continue;

        char *flag = argv[i] + ARG_MIN_DASH;
        arg *arg_target = args_find_arg_long(args, args_size, flag);

        if (!arg_target)
            continue;

        switch (arg_target->arg_value_type) {
        case ARG_VALUE_BOOL:
            *(bool *)*arg_target->arg_value_ptr = true;
            break;
        case ARG_VALUE_BOOL_TOGGLE:
            *(bool *)*arg_target->arg_value_ptr = !*(bool *)*arg_target->arg_value_ptr;
            break;
        case ARG_VALUE_STR:
            *arg_target->arg_value_ptr = argv[++i];
            break;
        }
    }
}

void args_parse_short(const int argc, char **argv, arg *args, size_t args_size)
{
    const size_t ARG_MIN_DASH = 1;

    for (int i = 1; i < argc; i++) {
        if (str_firstch(argv[i], '-') != ARG_MIN_DASH)
            continue;

        char *val = argv[i] + ARG_MIN_DASH;
        size_t val_len = strlen(val);
        bool is_multiple_bool = false;

        /* since we are parsing
         * short arguments with multiple flags in a single dash,
         * cut and parse it character-by-character.
         */
        for (unsigned int j = 0; j < val_len; j++) {
            const char flag[2] = {val[j], '\0'};
            arg *arg_target = args_find_arg_short(args, args_size, flag);

            if (!arg_target)
                continue;

            switch (arg_target->arg_value_type) {
            case ARG_VALUE_BOOL:
                is_multiple_bool = true;
                *(bool *)*arg_target->arg_value_ptr = true;
                break;
            case ARG_VALUE_BOOL_TOGGLE:
                is_multiple_bool = true;
                *(bool *)*arg_target->arg_value_ptr = !*(bool *)*arg_target->arg_value_ptr;
                break;
            case ARG_VALUE_STR:
                /* possible arg that has value 
                 * misplaced with multiple toggle args
                 *
                 * this only occurs on single dash arguments
                 */
                if (is_multiple_bool)
                    break;

                *arg_target->arg_value_ptr = argv[++i];
                break;
            }
        }
    }
}

void args_init(const int argc, char **argv, arg *args, size_t args_size)
{
    for (size_t i = 0; i < args_size; i++) {
        switch (args[i].arg_value_type) {
        case ARG_VALUE_BOOL:
        case ARG_VALUE_BOOL_TOGGLE:
            *args[i].arg_value_ptr = malloc(sizeof(bool));
            *(bool *)*args[i].arg_value_ptr = false;
            break;
        case ARG_VALUE_STR:
            *args[i].arg_value_ptr = NULL;
            break;
        }
    }

    args_parse_short(argc, argv, args, args_size);
    args_parse_long(argc, argv, args, args_size);
}

void args_free(arg *args, size_t args_size) 
{
    for (size_t i = 0; i < args_size; i++)
        if (args[i].arg_value_type == ARG_VALUE_BOOL || args[i].arg_value_type == ARG_VALUE_BOOL_TOGGLE)
            free(*args[i].arg_value_ptr);
}

void __args_debug(arg *args, size_t args_size)
{
    for (size_t i = 0; i < args_size; i++) {
        char **flag = args[i].valid_args + (ARR_SIZE(args[i].valid_args) - 1);
        for (; *flag == NULL; flag--);

        printf("[INFO]: argument '%s': ", *flag);

        switch (args[i].arg_value_type) {
        case ARG_VALUE_BOOL:
        case ARG_VALUE_BOOL_TOGGLE:
            puts(*((bool *)*args[i].arg_value_ptr) ? "true" : "false");
            break;
        case ARG_VALUE_STR:
            puts((char *)*args[i].arg_value_ptr ? (char *)*args[i].arg_value_ptr : "NULL");
            break;
        }
    }
}
