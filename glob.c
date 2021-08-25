#include "glob.h"

#include <stdlib.h>
#include <string.h>

#include "basics.h"

char *current_user_name;
char *userpathname = "/home/";

void init_glob()
{
    current_user_name = getenv("USER");
    //strcat(userpathname,current_user_name);
    userpathname = (char *)malloc(strlen(userpathname) + strlen(current_user_name) * sizeof(char *));
    strcat(userpathname,userpathname);
}