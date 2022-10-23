#include <stdlib.h>

#include "twrap_buf.h"

twrap_buf *twrap_gbuf;

void twrap_buf_init()
{
    twrap_gbuf = malloc(sizeof *twrap_gbuf);
    twrap_gbuf->bytes = (sizeof *twrap_gbuf->buf) * 2048;
    twrap_gbuf->buf = malloc(twrap_gbuf->bytes);
    twrap_gbuf->size = 0;
}

void twrap_buf_grow()
{
    int grow_multiplier = 1;

    while ((twrap_gbuf->bytes * grow_multiplier) <= twrap_gbuf->size)
        grow_multiplier++;

    twrap_gbuf->bytes = ((sizeof *twrap_gbuf->buf) * (twrap_gbuf->size * grow_multiplier));
    twrap_gbuf->buf = realloc(twrap_gbuf->buf, twrap_gbuf->bytes);
}

void twrap_buf_free()
{
    free(twrap_gbuf->buf);
    free(twrap_gbuf);
}
