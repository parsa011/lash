#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "basics.h"
#include "lsh.h"
#include "prompt.h"

char* prompt;
bool showpath;

void write_prompt()
{
    if (showpath){
        char currentdir[100];
        getcwd(currentdir,sizeof(currentdir));
        print(currentdir);
    }
    print(prompt);
}

void set_prompt(char *prm)
{
    prompt = prm;
}

char *get_prompt()
{
    return prompt;
}
