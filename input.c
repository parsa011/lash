#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "basics.h"
#include "builtin.h"
#include "prompt.h"
#include "lash.h"

/*
 *  brief Read a line of input from stdin.
 *  return The line from stdin.
 */
char *lash_read_line() {
    int bufsize = LASH_RL_BUFSIZE, c;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);

    // If we hit EOF, replace it with a null character and return.
    while (TRUE) {
        c = getchar();

        if (c == EOF || c == EOL) {
            buffer[position++] = '\0';
            return buffer;
        } else
            buffer[position++] = c;
    }

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
        bufsize += LASH_RL_BUFSIZE;
        buffer = realloc(buffer, bufsize);
    }
}

/*
 *  brief Split a line into tokens (very naively).
 *  param line The line.
 *  return Null-terminated array of tokens.
 */
char **lash_split_line(char *line) {
    int bufsize = LASH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    token = strtok(line, LASH_TOK_DELIM);
    // i have to write a tokenizer for \ / and ....
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += LASH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
        }
        token = strtok(NULL, LASH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

/*
 *  Utility Function
 *  brief find special chracter in line like > < | and return that
 */
char find_special_char(char *line) {
    while (*line++) {
        switch (*line) {
            case '>':
                return *line;
            case '<':
                return *line;
            case '|':
                return *line;
        }
    }
    return '\0';
}
