#define bool char
extern char* prompt;
extern bool showpath;

void write_prompt();
void set_prompt(char *);
char *get_prompt();
