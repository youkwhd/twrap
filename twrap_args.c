#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "twrap_args.h"

void twrap_args_init(const int argc, char **argv, twrap_arg *args, size_t args_size)
{
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            char *val = argv[i];
            while (*val == '-')
                val++;

            /* TODO: binary search
             */
            for (size_t j = 0; j < args_size; j++) {
                if (args[j].valid_args[1] != NULL && strcmp(val, args[j].valid_args[1]) == 0) {
                    if (args[j].arg_type == ARG_TOGGLE && *args[j].arg_value_ptr == NULL) {
                        bool *tmp_ptr = malloc(sizeof *tmp_ptr);
                        *tmp_ptr = true;
                        *args[j].arg_value_ptr = (void *)tmp_ptr;
                    } else if (args[j].arg_type == ARG_VALUE) {
                        *args[j].arg_value_ptr = argv[i + 1];
                    }

                    break;
                }
            }

        } else if (argv[i][0] == '-') {
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
                    if (args[j].valid_args[0] != NULL && strcmp(flag, args[j].valid_args[0]) == 0) {
                        if (args[j].arg_type == ARG_TOGGLE && *args[j].arg_value_ptr == NULL) {
                            bool *tmp_ptr = malloc(sizeof *tmp_ptr);
                            *tmp_ptr = true, is_multiple_toggle = true;
                            *args[j].arg_value_ptr = (void *)tmp_ptr;
                        } else if (args[j].arg_type == ARG_VALUE && !is_multiple_toggle) {
                            *args[j].arg_value_ptr = argv[i + 1];
                        }

                        break;
                    }
                }

                val_it++;
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
