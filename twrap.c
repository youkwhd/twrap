#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "twrap_buf.h"

void twrap_init(FILE *fp);
void twrap_free();

int main(void)
{
    twrap_init(stdin);

    for (size_t i = 0, count = 0; __twrap_buf->buf[i] != '\0'; i++) {
        bool count_reset = false;

        if (__twrap_buf->buf[i] == '\n') {
            count_reset = true;
        } else if (__twrap_buf->buf[i] == ' ' && count + (twrap_word_length(&__twrap_buf->buf[i + 1]) + 1) >= COUNT_BREAK) {
            __twrap_buf->buf[i] = '\n';
            count_reset = true;
        }

        putchar(__twrap_buf->buf[i]);
        count_reset ? count = 0 : count++;
    }

    twrap_free();
    return 0;
}


void twrap_init(FILE *fp)
{
    twrap_buf_init();

    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (__twrap_buf->size > __twrap_buf->bytes)
            twrap_buf_grow();

        __twrap_buf->buf[__twrap_buf->size++] = c;
    }
}

void twrap_free()
{
    twrap_buf_free();
}
