//  Function Declarations for builtin shell commands:
int lash_cd(char **args);
int lash_help(char **args);
int lash_exit(char **args);
int lash_num_builtins();

extern char *builtin_str[];

extern int (*builtin_func[])(char **);