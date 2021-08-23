#include <stdlib.h>

char *current_user_name;

void init_glob()
{
    current_user_name = getenv("USER");
}
