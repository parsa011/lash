#include "lash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "basics.h"
#include "builtin.h"
#include "prompt.h"
#include "input.h"

int main(int argc, char **arcv) {
    // load config file
    set_prompt("\033[033m%p\033[0m -- %t \n >  ");
    
    // Run loop
    lash_loop();

    return EXIT_SUCCESS;
}

void lash_loop(void) {
    char *line;
    char **args;
    int count;
    int status = 1;
    do {
        // writing the prompt
        write_prompt();
        count = 0;
        line = lash_read_line();
        args = lash_split_line(line,&count);
        printf("%d\n",count);
        for (int i  =0 ; i < count;i++)
            printl(args[i]);
        //status = lash_execute(args);

        free(line);
        free(args);
    } while (status);
}

/*
 *  brief Launch a program and wait for it to terminate.
 *  param args Null terminated list of arguments (including program).
 *  return Always returns 1, to continue execution.
 */
int lash_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("lash");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("lash");
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
int lash_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < lash_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return lash_launch(args);
}
