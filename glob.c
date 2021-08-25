#include "glob.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib/string/strlib.h"

char *current_user_name;
char *user_home_path;

void init_glob()
{
    current_user_name = getenv("USER");
    user_home_path = concat(3,"/home/",current_user_name,"/"); 
}