#include <time.h>
#include <stdio.h>
#include <sys/time.h>



// Set and start the timer
void timer_start(void);

// Stop and split the timer
void timer_stop(void);

// Get the time elapsed between timer_start and timer_stop
long timer_get(void);

// Print the time elapsed between timer_start and timer_stop
void timer_print(long int* res);

// Reset the timer (set it all to 0)
void timer_reset(void);

