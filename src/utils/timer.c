#include "timer.h"
#include <bits/time.h>
#include <time.h>

void timer_start(Timer *timer) {
  clock_gettime(CLOCK_MONOTONIC, &timer->start_time);
  timer->end_time = timer->start_time;
}

void timer_stop(Timer *timer) {
  clock_gettime(CLOCK_MONOTONIC, &timer->end_time);
}

double timer_get_seconds(Timer *timer) {
  return (timer->end_time.tv_sec - timer->start_time.tv_sec) + (timer->end_time.tv_nsec - timer->start_time.tv_nsec) / 1000000000.0;
}

