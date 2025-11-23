#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/* Utility functions */
void utils_init_random(void);
double utils_random_double(double min, double max);
double utils_abs(double x);
int utils_double_equal(double a, double b, double epsilon);

/* Timing utilities */
typedef struct {
    long long start_time;
    long long end_time;
} Timer;

void timer_start(Timer *t);
void timer_stop(Timer *t);
double timer_elapsed_ms(const Timer *t);

#endif /* UTILS_H */
