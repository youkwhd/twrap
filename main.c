#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define BUF_SIZE 4096 * 5

char *twrap_buf;
size_t twrap_size = 0;

void twrap_buf_init()
{
    twrap_buf = malloc((sizeof *twrap_buf) * BUF_SIZE);
}

void twrap_buf_free()
{
    free(twrap_buf);
}

void twrap_init(FILE *fp)
{
    twrap_buf_init();

    char c;
    while ((c = fgetc(fp)) != EOF)
        twrap_buf[twrap_size++] = c;
    twrap_buf[twrap_size - 1] = '\0';
}

void twrap_free()
{
    twrap_buf_free();
}

size_t twrap_word_length(const char *word)
{
    size_t length = 0;

    for (; *word; word++, length++)
        if (*word == ' ')
            break;

    return length;
}

int main(int argc, char **argv)
{
    /* FILE *fp = argc >= 2 ? fopen(argv[1], "r") : stdin; */
    twrap_init(stdin);

    for (int i = 0; twrap_buf[i] != '\0'; i++) {
        if (twrap_buf[i] == ' ') { 
            printf("%zu\n", twrap_word_length(twrap_buf + (i + 1)));
        }
    }

    twrap_free();
    return 0;
}
