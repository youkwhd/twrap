#include <stdlib.h>

#include "buf.h"

buf *gbuf;

void buf_init()
{
    gbuf = malloc(sizeof *gbuf);
    gbuf->bytes = (sizeof *gbuf->buf) * 2048;
    gbuf->buf = malloc(gbuf->bytes);
    gbuf->size = 0;
}

void buf_fread(FILE *fp)
{
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (gbuf->size > gbuf->bytes)
            buf_grow();

        gbuf->buf[gbuf->size++] = c;
    }
}

void buf_grow()
{
    int grow_multiplier = 1;

    while ((gbuf->bytes * grow_multiplier) <= gbuf->size)
        grow_multiplier++;

    gbuf->bytes = ((sizeof *gbuf->buf) * (gbuf->size * grow_multiplier));
    gbuf->buf = realloc(gbuf->buf, gbuf->bytes);
}

void buf_free()
{
    free(gbuf->buf);
    free(gbuf);
}
