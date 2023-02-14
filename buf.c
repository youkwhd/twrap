#include <stdlib.h>

#include "buf.h"

buf *buf_init()
{
    buf *buffer = malloc(sizeof *buffer);
    buffer->bytes = (sizeof *buffer->content) * BUF_INITIAL_SIZE;
    buffer->content = malloc(buffer->bytes);
    buffer->size = 0;

    return buffer;
}

void buf_fread(buf *buffer, FILE *fp)
{
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (buffer->size > buffer->bytes)
            buf_grow(buffer);

        buffer->content[buffer->size++] = c;
    }
}

void buf_grow(buf *buffer)
{
    size_t grow_multiplier = 1;

    while ((buffer->bytes * grow_multiplier) <= buffer->size)
        grow_multiplier++;

    buffer->bytes = ((sizeof *buffer->content) * (buffer->size * grow_multiplier));
    buffer->content = realloc(buffer->content, buffer->bytes);
}

void buf_free(buf *buffer)
{
    free(buffer->content);
    free(buffer);
}
