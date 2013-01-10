/*
 * libtime
 *
*/
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <malloc.h>
#include <fdict/libtime.h>

int getcurmillisec()
{
    struct timeval clock;
    
    gettimeofday(&clock, NULL);
    return 1000*clock.tv_sec + clock.tv_usec/1000;
}

void timestart(time_info* ti)
{
  gettimeofday(&ti->clockstart, NULL);  
}

int timeend(time_info* ti)
{
  gettimeofday(&ti->clockend, NULL);
  ti->interval_time = 1000*(ti->clockend.tv_sec - ti->clockstart.tv_sec) + (ti->clockend.tv_usec - ti->clockstart.tv_usec)/1000;
  return ti->interval_time;
}

int timeend_usec(time_info* ti)
{
  gettimeofday(&ti->clockend, NULL);
  ti->interval_time = 1000000*(ti->clockend.tv_sec - ti->clockstart.tv_sec) + (ti->clockend.tv_usec - ti->clockstart.tv_usec);
  return ti->interval_time;
}

void restart_timeinfo(time_info* ti)
{
  reset_timeinfo(ti);
  timestart(ti);
}

void reset_timeinfo(time_info* ti)
{
  if (ti == NULL)
    return ;
  ti->interval_time = 0;
}
