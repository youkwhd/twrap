#ifndef TWRAP_BUF_H
#define TWRAP_BUF_H

#include <stdio.h>
#include <stddef.h>

typedef struct twrap_buf {
    char *buf;
    size_t size;
    size_t bytes;
} twrap_buf;

/* twrap global buffer
 * allocated after twrap_buf_init() called
 */
extern twrap_buf *twrap_gbuf;

void twrap_buf_init();
void twrap_buf_grow();
void twrap_buf_free();
size_t twrap_word_length(const char *word);

#endif
