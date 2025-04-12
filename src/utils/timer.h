#pragma once

#include <time.h>

typedef struct {
  struct timespec start_time;
  struct timespec end_time;
} Timer;

void timer_start(Timer *timer);
void timer_stop(Timer *timer);
double timer_get_seconds(Timer *timer);
