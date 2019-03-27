#include <stdio.h>
#include <time.h>

int main ()
{
   time_t rawtime;
   struct tm *info;
   char buffer[80];

   time( &rawtime );

   info = localtime( &rawtime );
   printf("Current local time and date: %s", asctime(info));
   printf("Current local date is %d %d\n", info->tm_mon + 1, info->tm_mday);
   printf("Current day of week is %d\n", info->tm_wday); // (0:Sunday … 6:Saturday)

   return(0);
}
