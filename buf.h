#ifndef __BUF_H
#define __BUF_H

#include <stdio.h>
#include <stddef.h>

#define BUF_INITIAL_SIZE 2048

#define buf_read(buffer) buf_fread(buffer, stdin)

typedef struct buf {
    char *buf;
    size_t size;
    size_t bytes;
} buf;

/* allocates gbuf
 */
buf *buf_init();

/* reads FILE 
 */
void buf_fread(buf *buffer, FILE *fp);

/* grow the size of gbuf
 */
void buf_grow(buf *buffer);

/* free the memory of gbuf
 */
void buf_free(buf *buffer);

#endif
