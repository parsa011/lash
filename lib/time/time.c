#include <stdio.h>      /* puts, printf */
#include <time.h>       /* time_t, struct tm, time, localtime */
#include "time.h"
#include "../string/strlib.h"
#include "basics.h"

char *getdate()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    return strrpl(asctime(timeinfo),"\n","");
}

char *gettime()
{   time_t rawtime;
   struct tm *info;
   char buffer[80];

   time( &rawtime );

   info = localtime( &rawtime );

   strftime(buffer,80,"%X", info);
    return strrpl(buffer,"\n","");
}
