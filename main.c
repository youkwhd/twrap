#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <unistd.h>

#define BUF_SIZE 4096 * 5

int main(int argc, char **argv)
{
    FILE *fp = argc >= 2 ? fopen(argv[1], "r") : stdin;

    /* TODO: convert number to string */
    /* size_t line_break_count = argc <= 2 ? argv[2] : argv[1]; */
    size_t line_break_count = 60;

    char buf[BUF_SIZE];
    size_t size = 0;

    char c;
    size_t lbreak_count = 0;
    while ((c = fgetc(fp)) != EOF) {
        buf[size++] = c;
        if (lbreak_count++ == line_break_count) {
            buf[size++] = '\n';
            lbreak_count = 0;
        }
        if (c == '\n')
            lbreak_count = 0;
    }
    printf("%s", buf);


    return 0;
}
