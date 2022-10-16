#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "twrap.h"

int main(void)
{
    twrap_init(stdin);

    for (size_t i = 0, count = 0; __twrap_buf->buf[i] != '\0'; i++) {
        /* ok, this is for count to be presicion. 
         * + speed.
         */
        bool set_count_zero = false;

        if (__twrap_buf->buf[i] == '\n')
            set_count_zero = true;
        else if (__twrap_buf->buf[i] == ' ' && count + (twrap_word_length(&__twrap_buf->buf[i + 1]) + 1) >= COUNT_BREAK) {
            __twrap_buf->buf[i] = '\n';
            set_count_zero = true;
        }

        putchar(__twrap_buf->buf[i]);
        count++;

        set_count_zero ? count = 0 : count;
    }

    twrap_free();
    return 0;
}
