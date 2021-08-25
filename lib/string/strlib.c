#include "strlib.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

char* strrpl(char* s, char* old,char* new)
{
    char* result;
    int i, cnt = 0;
    int newlen = strlen(new);
    int oldlen = strlen(old);
  
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], old) == &s[i]) {
            cnt++;
  
            // Jumping to index after the old word.
            i += oldlen - 1;
        }
    }
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newlen - oldlen) + 1);
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, old) == s) {
            strcpy(&result[i], new);
            i += newlen;
            s += oldlen;
        }
        else
            result[i++] = *s++;
    }
  
    result[i] = '\0';
    return result;
}

int lastindexof(char *s,char n) {
    for (int i = strlen(s) - 1;i > 0;i--) {
        if (s[i] == n)
            return i; 
    }
    return -1;
}

int indexof(char *s,char n) {
    char *e = strchr(s, n);
    if (e == NULL) {
        return -1;
    }
    return (int)(e - s);
}

int countchar(char* s, char x)             
{
    int i,count,lngth;
    lngth = strlen(s);
    count = 0;
    for (i = 0; i < lngth ; i++){if (s[i] == x) count++; }
    return count;
}

char *concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}
