#ifndef __ARGS_H
#define __ARGS_H

#include <stddef.h>

#define args_find_arg_long(args, args_size, arg_flag) args_find_arg(args, args_size, arg_flag, ARG_TYPE_LONG)
#define args_find_arg_short(args, args_size, arg_flag) args_find_arg(args, args_size, arg_flag, ARG_TYPE_SHORT)

typedef enum arg_val_type {
    ARG_VALUE_STR,
    ARG_VALUE_BOOL,
    ARG_VALUE_BOOL_TOGGLE
} arg_val_type;

typedef enum arg_type {
    ARG_TYPE_SHORT,
    ARG_TYPE_LONG
} arg_type;


typedef struct arg {
    char *valid_args[2];
    arg_val_type arg_value_type;
    void **arg_value_ptr;
} arg;

void args_init(const int argc, char **argv, arg *args, size_t args_size);
void args_free(arg *args, size_t args_size);
arg *args_find_arg(arg *args, size_t args_size, const char *arg_flag, arg_type arg_type);
void __args_debug(arg *args, size_t args_size);

#endif
