#include <stdio.h>
#include "str.h"

size_t str_wordlen(const char *word)
{
    size_t length = 0;

    for (; *word; word++, length++)
        if (*word == ' ' || *word == '\n')
            break;

    return length;
}

size_t str_firstch(const char *str, char ch)
{
    size_t count = 0;

    for (; *str == ch; str++)
        count++;

    return count;
}
