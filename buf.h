#ifndef __BUF_H
#define __BUF_H

#include <stdio.h>
#include <stddef.h>

#define BUF_INITIAL_SIZE 2048

#define buf_read(buffer) buf_fread(buffer, stdin)

typedef struct buf {
    char *content;
    size_t size;
    size_t bytes;
} buf;

/* allocates buf
 */
buf *buf_init();

/* reads FILE into buf
 */
void buf_fread(buf *buffer, FILE *fp);

/* grow the size of buf
 */
void buf_grow(buf *buffer);

/* free the memory of buf
 */
void buf_free(buf *buffer);

#endif
