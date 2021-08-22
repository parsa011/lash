#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "basics.h"
#include "lash.h"
#include "prompt.h"
#include "lib/string/strlib.h"

char* prompt;
bool showpath;

void write_prompt()
{
    print(eval_prompt(prompt));
}

void set_prompt(char *prm)
{
    prompt = prm;
}

char *get_prompt()
{
    return prompt;
}

char *eval_prompt()
{
    char currentdir[100];
    getcwd(currentdir,sizeof(currentdir));
    char *res = strrpl(prompt,"%d",currentdir);
    return res;
}