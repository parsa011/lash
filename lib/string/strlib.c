#include <string.h>
#include "strlib.h"
#include <stdlib.h>

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
    for (int i = 0;i < strlen(s);i++) {
        if (*s++ == n)
            return i;
    }
    return -1;
}
