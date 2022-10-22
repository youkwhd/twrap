#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "twrap_args.h"

void twrap_args_init(const int argc, char **argv, twrap_arg *args, size_t args_size)
{
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' || (argv[i][0] != '-' && argv[i][1] != '-'))
            continue;

        char *val = argv[i];
        while (*val == '-')
            val++;

        for (int j = 0; j < (int)args_size; j++) {
            for (int k = 0; k < 2; k++) {
                if (args[j].valid_args[k] != NULL && strcmp(val, args[j].valid_args[k]) == 0) {
                    if (args[j].arg_type == ARG_TOGGLE) {
                        bool *ptr = malloc(sizeof *ptr);
                        *ptr = true;
                        *args[j].arg_value_ptr = (void *)ptr;
                    } else if (args[j].arg_type == ARG_VALUE) {
                        *args[j].arg_value_ptr = argv[++i];
                    }

                    j = args_size;
                    break;
                }
            }
        }
    }
}

void twrap_args_free(twrap_arg *args, size_t args_size) 
{
    for (int i = 0; i < (int)args_size; i++)
         if (args[i].arg_type == ARG_TOGGLE && args[i].arg_value_ptr != NULL && *args[i].arg_value_ptr != NULL)
             free(*args[i].arg_value_ptr);
}
