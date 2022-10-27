#ifndef __BUF_H
#define __BUF_H

#include <stdio.h>
#include <stddef.h>

#define buf_read() buf_fread(stdin)

typedef struct buf {
    char *buf;
    size_t size;
    size_t bytes;
} buf;

/* twrap global buffer
 * allocated after buf_init() called
 */
extern buf *gbuf;

/* allocates gbuf
 */
void buf_init();

/* reads FILE 
 */
void buf_fread(FILE *fp);

/* grow the size of gbuf
 */
void buf_grow();
/* free the memory of gbuf
 */
void buf_free();


#endif
