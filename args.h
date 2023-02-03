#ifndef __ARGS_H
#define __ARGS_H

#include <stddef.h>

typedef enum arg_type {
    ARG_VALUE,
    ARG_TOGGLE
} arg_type;

typedef struct arg {
    char *valid_args[2];
    arg_type arg_type;
    void **arg_value_ptr;
} arg;

void args_init(const int argc, char **argv, arg *args, size_t args_size);
void args_free(arg *args, size_t args_size);
void __args_debug(arg *args, size_t args_size);

#endif
