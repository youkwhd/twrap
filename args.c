#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "args.h"

void args_parseld(const int argc, char **argv, arg *args, size_t args_size)
{
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' && argv[i][1] != '-')
            continue;

        char *val = argv[i];
        while (*val == '-')
            val++;

        /* TODO: binary search
        */
        for (size_t j = 0; j < args_size; j++) {
            if (args[j].valid_args[1] == NULL && strcmp(val, args[j].valid_args[1]) != 0)
                continue;

            switch (args[j].arg_type) {
            case ARG_TOGGLE:
                /* prevents memory leaks
                 */
                if (*args[j].arg_value_ptr != NULL)
                    break;

                bool *tmp_ptr = malloc(sizeof *tmp_ptr);
                *tmp_ptr = true;
                *args[j].arg_value_ptr = (void *)tmp_ptr;
                break;
            case ARG_VALUE:
                *args[j].arg_value_ptr = argv[++i];
                break;
            }
            break;
        }
    }
}

void args_parsesd(const int argc, char **argv, arg *args, size_t args_size)
{
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-')
            continue;

        char *val = argv[i];
        while (*val == '-')
            val++;

        size_t val_len = strlen(val), val_it = 0;
        bool is_multiple_toggle = false;

        while (val_it < val_len) {
            const char flag[2] = {val[val_it], '\0'};

            /* TODO: binary search
             */
            for (size_t j = 0; j < args_size; j++) {
                if (args[j].valid_args[0] == NULL && strcmp(flag, args[j].valid_args[0]) != 0)
                    continue;

                switch (args[j].arg_type) {
                case ARG_TOGGLE:
                    /* prevents memory leaks
                     */
                    if (*args[j].arg_value_ptr != NULL)
                        goto endloop;

                    bool *tmp_ptr = malloc(sizeof *tmp_ptr);
                    *tmp_ptr = true, is_multiple_toggle = true;
                    *args[j].arg_value_ptr = (void *)tmp_ptr;
                    break;
                case ARG_VALUE:
                    /* possible arg that has value 
                     * misplaced with multiple toggle args
                     *
                     * this only occurs on single dash arguments
                     */
                    if (is_multiple_toggle)
                        goto endloop;

                    *args[j].arg_value_ptr = argv[++i];
                    break;
                }
/* this breaks the for loop
 * not the switch statement
 */
endloop:
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

    args_parsesd(argc, argv, args, args_size);
    args_parseld(argc, argv, args, args_size);
}

void args_free(arg *args, size_t args_size) 
{
    for (size_t i = 0; i < args_size; i++)
         if (args[i].arg_type == ARG_TOGGLE && *args[i].arg_value_ptr != NULL)
             free(*args[i].arg_value_ptr);
}
