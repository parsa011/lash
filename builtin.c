#include "builtin.h"

#include <stdio.h>
#include <stdlib.h>

#include "basics.h"

/*
 * builtin function as char array , for show in help and get by name
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"};

/*
 *  List of builtin commands, followed by their corresponding functions.
 */
int (*builtin_func[])(char **) = {
    &lash_cd,
    &lash_help,
    &lash_exit};

int lash_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
 *  Builtin function implementations.
 */
int lash_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lash: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lash");
        }
    }
    return 1;
}

int lash_help(char **args) {
    int i;
    printl(
    "author : pms (parsa mahmoudy sahebi || parsa011)\n contributer : \n Ali (alichraghi)\n"
    "Type program names and arguments, and hit enter.\n"
    "The following are built in:"
    );

    for (i = 0; i < lash_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int lash_exit(char **args) {
    exit(0);
    return 0;
}
