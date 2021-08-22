#include <stdlib.h>
#include <stdio.h>
#include "basics.h"
#include "builtin.h"

/*
 * builtin function 
 */
extern char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

/*
 * some pointer to builtin functions
 */
extern int (*builtin_func[]) (char **) = {
    &lash_cd,
    &lash_help,
    &lash_exit
};

int lash_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}
/*
 *  Builtin function implementations.
 */
int lash_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "lash: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lash");
        }
    }
    return 1;
}

int lash_help(char **args)
{
    int i;
    printl("modified and improved lsh by pms (parsa mahmoudy sahebi , parsa011 :D)");
    printl("Type program names and arguments, and hit enter.");
    printl("The following are built in:");

    for (i = 0; i < lash_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int lash_exit(char **args)
{
    return 0;
}

// after every time allocation , will check the target to be allocated
void check_alloc(char *b)
{
    if (!b){
        fprintf(stderr,"lash : allocation error !");
        exit(EXIT_FAILURE);
    }
}
