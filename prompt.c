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

char *eval_prompt() {
    char currentdir[100];
    getcwd(currentdir, sizeof(currentdir));
    char *res = strrpl(prompt, "%d", currentdir);
    char *currenttime = gettime();
    char *temp = strrpl(res, "%t", currenttime);
    return temp;
}
