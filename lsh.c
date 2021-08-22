#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "basics.h"
#include "lsh.h"
#include "prompt.h"
#include "builtin.h"

int main(int argc,char **arcv)
{
    // load config file
    set_prompt(">  ");
    showpath = TRUE;
    // Run loop
    lsh_loop();

    return EXIT_SUCCESS;
}

void lsh_loop(void)
{
    char *line;
    char **args;
    int status;
    do{
        // writing the prompt
        write_prompt();
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    }while (status);
}

/*
 *  brief Read a line of input from stdin.
 *  return The line from stdin.
 */
char *lsh_read_line()
{
    int bufsize = LSH_RL_BUFSIZE, c;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);

    check_alloc(buffer);
    
    // If we hit EOF, replace it with a null character and return.
    while (TRUE) {
        c = getchar();

        if (c == EOF || c == EOL){
            buffer[position++] = '\0';
            return buffer;
        }else 
            buffer[position++] = c;
    }

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize){
        bufsize += LSH_RL_BUFSIZE;
        buffer = realloc(buffer,bufsize);
        check_alloc(buffer);
    }
}

/*
 *  brief Split a line into tokens (very naively).
 *  param line The line.
 *  return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize *  sizeof(char*));
    char *token;

    token = strtok(line,LSH_TOK_DELIM);
    // i have to write a tokenizer for \ / and ....
    while (token != NULL){
        tokens[position++] = token;

        if (position >= bufsize){
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens,bufsize * sizeof(char*));
        }
        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

/*
 *  brief Launch a program and wait for it to terminate.
 *  param args Null terminated list of arguments (including program).
 *  return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("lsh");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/*
 *  brief Execute shell built-in or launch program.
 *  param args Null terminated list of arguments.
 *  return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
}