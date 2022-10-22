#ifndef TWRAP_ARGS_H
#define TWRAP_ARGS_H

#include <stddef.h>

typedef enum twrap_arg_type {
    ARG_VALUE,
    ARG_TOGGLE
} twrap_arg_type;

typedef struct twrap_arg {
    char *valid_args[2];
    twrap_arg_type arg_type;
    void **arg_value_ptr;
} twrap_arg;

void twrap_args_init(const int argc, char **argv, twrap_arg *args, size_t args_size);
void twrap_args_free(twrap_arg *args, size_t args_size);

#endif
