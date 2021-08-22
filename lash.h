#define LASH_RL_BUFSIZE 1024
#define LASH_TOK_BUFSIZE 64
#define LASH_TOK_DELIM " \t\r\n\a"

void lash_loop(void);
char *lash_read_line(void);
char **lash_split_line(char *line);
int lash_launch(char **args);
int lash_execute(char **args);