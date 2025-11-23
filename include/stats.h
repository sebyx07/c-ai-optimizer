#ifndef STATS_H
#define STATS_H

#include <stddef.h>

/* Statistical functions */
double stats_mean(const double *data, size_t n);
double stats_variance(const double *data, size_t n);
double stats_stddev(const double *data, size_t n);
double stats_min(const double *data, size_t n);
double stats_max(const double *data, size_t n);
void stats_sort(double *data, size_t n);

/* Advanced statistics */
double stats_median(double *data, size_t n);
double stats_correlation(const double *x, const double *y, size_t n);

#endif /* STATS_H */
