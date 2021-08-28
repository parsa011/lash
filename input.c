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
#include "history.h"
#include "lib/string/strlib.h"
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

            // add to history
            add_to_history(buffer,20);

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
char **lash_split_line(char *line,int *count) {

    int bufsize = LASH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    char *special;
    int specialcharindex;

    token = strtok(line,LASH_TOK_DELIM);
    /*
     *  if token has special char
     *  will find the index of special char
     *  if that was at the begining of the token will add special to our list 
     *  if that was and the end of token , will remove that from the end and add both token and special char
     *  and if that was in middle , will make token to 2 different pieces and both to our tokends
     */
    while (token != NULL) {
        special = find_special_char(token);

        if (special != NULL) {
            specialcharindex = indexof(token,special[0]);       
            if (specialcharindex == -1) {
                tokens[position++] = token;
            }
            else if (specialcharindex == 0) {
                tokens[position++] = special;
                token++;
            } else if(specialcharindex == strlen(token) - 1) {
                token[strlen(token)-1] = '\0';
                tokens[position++] = token;
                tokens[position++] = special;
            }
            else {
                char* firsthalf = malloc(LASH_TOK_BUFSIZE * sizeof(char*));
                char* secondhalf = malloc(LASH_TOK_BUFSIZE* sizeof(char*));
                strcpy(firsthalf,token);
                strcpy(secondhalf,token);

                // Get First Half
                firsthalf[specialcharindex] = '\0';
                tokens[position++] = firsthalf;

                // Add in special
                tokens[position++] = special;

                // Get Second Half
                secondhalf = strstr(secondhalf,special);
                secondhalf++;
                tokens[position++] = secondhalf;

                token = secondhalf;
            }

        }
        else{
            tokens[position++] = token;
        }

        token = strtok(NULL,LASH_TOK_DELIM);

        if (position >= bufsize) {
            bufsize += LASH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
        }
    }
    tokens[position] = NULL;
    *count = position;
    return tokens;
}

/*
 *  Utility Function
 *  brief find special chracter in line like > < | and return that
 */
char *find_special_char(char *line) {
    while (*line++) {
        switch (*line) {
            case '>':
                return ">\0";
            case '<':
                return "<\0";
            case '|':
                return "|\0";
            case ';':
                return ";\0";
        }
    }
    return NULL; 
}

bool isspecial(char *c) {
    switch (*c) {
        case '>':
        case '<':
        case '|':
        case ';':
            return TRUE;
        default:
            return FALSE;
    }
}
