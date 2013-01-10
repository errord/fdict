/*
 * libtime
 *
*/
#ifndef _CLIB_LIBTIME_H_
#define _CLIB_LIBTIME_H_

struct time_info_s
{
  struct timeval clockstart;
  struct timeval clockend;
  int interval_time;
};
typedef struct time_info_s time_info;

/* get current millisec */
int getcurmillisec();

/* time start */
void timestart(time_info* ti);

/* time end, return interval time msec */
int timeend(time_info* ti);

/* time end, return interval time usec */
int timeend_usec(time_info* ti);

/* restart time struct */
void restart_timeinfo(time_info* ti);

/* reset time struct */
void reset_timeinfo(time_info* ti);

#ifdef DEBUG
#define DEBUG_TIME_DEFINE time_info timeinfo
#define DEBUG_TIME_START timestart(&timeinfo)
#define DEBUG_TIME_END timeend(&timeinfo)
#define DEBUG_OUT_INFO(info) printf(info)
#define DEBUG_OUT_INFO_A1(info,a1) printf(info,a1)
#define DEBUG_OUT_TIMEINFO(info) printf(info, timeinfo.interval_time)
#define DEBUG_OUT_TIMEINFO_A1(info,a1) printf(info, a1, timeinfo.interval_time)
#define DEBUG_TIME_RESET reset_timeinfo(&timeinfo)
#else
#define DEBUG_TIME_DEFINE
#define DEBUG_TIME_START
#define DEBUG_TIME_END
#define DEBUG_OUT_INFO(info)
#define DEBUG_OUT_INFO_A1(info,a1)
#define DEBUG_OUT_TIMEINFO(info)
#define DEBUG_OUT_TIMEINFO_A1(info,a1)
#define DEBUG_TIME_RESET
#endif /* DEBUG */

#endif /* _CLIB_LIBTIME_H_ */

