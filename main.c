#include <stdio.h>
#include "twrap.h"

int main(int argc, char **argv)
{
    twrap_init(stdin);

    for (int i = 0, count = 0; __twrap_buf->buf[i] != '\0'; i++, count++) {
        if (__twrap_buf->buf[i] == ' ' && (count + twrap_word_length(&__twrap_buf->buf[i + 1]) > COUNT_BREAK)) {
            putchar('\n');
            count = 0;
            i++;
        }

        putchar(__twrap_buf->buf[i]);
    }

    twrap_free();
    return 0;
}
