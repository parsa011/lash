#include "filelib.h"

#include <stdio.h>

int file_linecount(FILE *fp) {
    if (fp == NULL)
        return -1;
    int count = 0;
    char chr = getc(fp);
    while (chr != EOF) {
        if (chr == 'n')
            count += 1;
        chr = getc(fp);
    }
    return count;
}
