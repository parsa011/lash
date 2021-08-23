#include <stdio.h>      /* puts, printf */
#include <time.h>       /* time_t, struct tm, time, localtime */
#include "time.h"
#include "../string/strlib.h"

char *gettime()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    return strrpl(asctime(timeinfo),"\n","");
}
