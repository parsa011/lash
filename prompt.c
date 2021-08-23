#include "prompt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "basics.h"
#include "lash.h"
#include "lib/string/strlib.h"
#include "lib/time/time.h"
#include "glob.h"

char *prompt;

void write_prompt() {
    print(eval_prompt(prompt));
}

void set_prompt(char *prm) {
    prompt = prm;
}

char *get_prompt() {
    return prompt;
}

char *eval_prompt()
{
    char *currentdir = getenv("PWD");
    char *res = strrpl(prompt,"%p",currentdir);
    res = strrpl(res,"%t",gettime());
    res = strrpl(res,"%d",getdate());
    res = strrpl(res,"%u",current_user_name);
    return res; 
}
