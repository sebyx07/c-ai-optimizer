#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

void utils_init_random(void) {
    srand((unsigned int)time(NULL));
}

double utils_random_double(double min, double max) {
    double random = (double)rand() / (double)RAND_MAX;
    return min + random * (max - min);
}

double utils_abs(double x) {
    return (x < 0.0) ? -x : x;
}

int utils_double_equal(double a, double b, double epsilon) {
    return utils_abs(a - b) < epsilon;
}

void timer_start(Timer *t) {
    if (t == NULL) {
        return;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);
    t->start_time = (long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec;
}

void timer_stop(Timer *t) {
    if (t == NULL) {
        return;
    }

    struct timeval tv;
    gettimeofday(&tv, NULL);
    t->end_time = (long long)tv.tv_sec * 1000000LL + (long long)tv.tv_usec;
}

double timer_elapsed_ms(const Timer *t) {
    if (t == NULL) {
        return 0.0;
    }

    long long diff = t->end_time - t->start_time;
    return (double)diff / 1000.0;
}
