#include "stats.h"
#include "utils.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

double stats_mean(const double *data, size_t n) {
    if (data == NULL || n == 0) {
        return 0.0;
    }

    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += data[i];
    }

    return sum / (double)n;
}

double stats_variance(const double *data, size_t n) {
    if (data == NULL || n == 0) {
        return 0.0;
    }

    double mean = stats_mean(data, n);
    double sum_sq_diff = 0.0;

    for (size_t i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_sq_diff += diff * diff;
    }

    return sum_sq_diff / (double)n;
}

double stats_stddev(const double *data, size_t n) {
    return sqrt(stats_variance(data, n));
}

double stats_min(const double *data, size_t n) {
    if (data == NULL || n == 0) {
        return 0.0;
    }

    double min_val = data[0];
    for (size_t i = 1; i < n; i++) {
        if (data[i] < min_val) {
            min_val = data[i];
        }
    }

    return min_val;
}

double stats_max(const double *data, size_t n) {
    if (data == NULL || n == 0) {
        return 0.0;
    }

    double max_val = data[0];
    for (size_t i = 1; i < n; i++) {
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }

    return max_val;
}

static int compare_doubles(const void *a, const void *b) {
    double diff = *(const double*)a - *(const double*)b;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

void stats_sort(double *data, size_t n) {
    if (data == NULL || n == 0) {
        return;
    }

    qsort(data, n, sizeof(double), compare_doubles);
}

double stats_median(double *data, size_t n) {
    if (data == NULL || n == 0) {
        return 0.0;
    }

    double *temp = (double*)malloc(n * sizeof(double));
    if (temp == NULL) {
        return 0.0;
    }

    memcpy(temp, data, n * sizeof(double));
    stats_sort(temp, n);

    double median;
    if (n % 2 == 0) {
        median = (temp[n/2 - 1] + temp[n/2]) / 2.0;
    } else {
        median = temp[n/2];
    }

    free(temp);
    return median;
}

double stats_correlation(const double *x, const double *y, size_t n) {
    if (x == NULL || y == NULL || n == 0) {
        return 0.0;
    }

    double mean_x = stats_mean(x, n);
    double mean_y = stats_mean(y, n);

    double sum_xy = 0.0;
    double sum_x_sq = 0.0;
    double sum_y_sq = 0.0;

    for (size_t i = 0; i < n; i++) {
        double dx = x[i] - mean_x;
        double dy = y[i] - mean_y;

        sum_xy += dx * dy;
        sum_x_sq += dx * dx;
        sum_y_sq += dy * dy;
    }

    double denominator = sqrt(sum_x_sq * sum_y_sq);
    if (denominator < 1e-10) {
        return 0.0;
    }

    return sum_xy / denominator;
}
