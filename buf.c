#include <stdlib.h>

#include "buf.h"

buf *buf_init()
{
    buf *buffer = malloc(sizeof *buffer);
    buffer->bytes = (sizeof *buffer->content) + BUF_INITIAL_SIZE;
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

    /* fix: windows for some reason does not
     * translate null terminator
     */
    if (buffer->content[buffer->size] != '\0')
        buffer->content[buffer->size++] = '\0';
}

void buf_grow(buf *buffer)
{
    buffer->bytes += BUF_INITIAL_SIZE;
    buffer->content = realloc(buffer->content, buffer->bytes);
}

void buf_free(buf *buffer)
{
    free(buffer->content);
    free(buffer);
}
