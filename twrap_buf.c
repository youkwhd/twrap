#include <stdlib.h>

#include "twrap_buf.h"

twrap_buf *__twrap_buf;

void twrap_buf_init()
{
    __twrap_buf = malloc(sizeof *__twrap_buf);
    __twrap_buf->bytes = (sizeof *__twrap_buf->buf) * 2048;
    __twrap_buf->buf = malloc(__twrap_buf->bytes);
    __twrap_buf->size = 0;
}

void twrap_buf_grow()
{
    size_t grow_size;
    int grow_multiplier = 1;

    while ((grow_size = __twrap_buf->bytes * grow_multiplier) <= __twrap_buf->size)
        grow_multiplier++;

    __twrap_buf->bytes = (sizeof *__twrap_buf->buf) * grow_size;
    __twrap_buf->buf = realloc(__twrap_buf->buf, __twrap_buf->bytes);
}

void twrap_buf_free()
{
    free(__twrap_buf->buf);
    free(__twrap_buf);
}

size_t twrap_word_length(const char *word)
{
    size_t length = 0;

    for (; *word; word++, length++)
        if (*word == ' ' || *word == '\n')
            break;

    return length;
}