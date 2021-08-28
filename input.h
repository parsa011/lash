#define LASH_RL_BUFSIZE 1024
#define LASH_TOK_BUFSIZE 64
#define LASH_TOK_DELIM " \t\r\n\a"

char *lash_read_line(void);
char **lash_split_line(char *,int *);

/* 
 *  Utilities
 */
char *find_special_char(char *);
char is_special(char *);
