#define bool char
extern char *prompt;

void write_prompt();
void set_prompt(char *);
char *get_prompt();
char *eval_prompt();