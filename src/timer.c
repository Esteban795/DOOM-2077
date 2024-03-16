#include "../include/timer.h"

struct timeval tval_before, tval_after, tval_result;
// Set and start the timer
void timer_start(void) {
  gettimeofday(&tval_before, NULL);
  return;
}

// Stop and split the timer
void timer_stop(void) {
  gettimeofday(&tval_after, NULL);
  return;
}

// Print the time elapsed between timer_start and timer_stop
void timer_print(long int* res) {
  timersub(&tval_after, &tval_before, &tval_result);
  long int temp = tval_result.tv_usec;
  printf("Time elapsed: %ldus\n",temp);
  *res += temp;
  return;
}