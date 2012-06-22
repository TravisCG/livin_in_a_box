#include <sys/time.h>
#include <time.h>
#include "cg_time.h"

double cg_start_time  = 0.0;

void startTime(){
   struct timeval start;

   gettimeofday(&start, NULL);
   cg_start_time = (double)start.tv_sec + (double)start.tv_usec / 1000000.0;
}

double getTimeInterval(){
   struct timeval time;

   gettimeofday(&time, NULL);

   return((double)time.tv_sec + 
          (double)time.tv_usec / 1000000.0 -
           cg_start_time);
}
