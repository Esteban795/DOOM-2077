#include <time.h>
#include <stdio.h>
#include <sys/time.h>



// Set and start the timer
void timer_start(void);

// Stop and split the timer
void timer_stop(void);

// Print the time elapsed between timer_start and timer_stop
void timer_print(long int* res);


