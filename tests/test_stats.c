#include "../include/stats.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

int test_stats_mean(void)
{
    double data[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double mean = stats_mean(data, 5);

    return utils_double_equal(mean, 3.0, 1e-6);
}

int test_stats_min_max(void)
{
    double data[] = {5.0, 2.0, 8.0, 1.0, 9.0};
    double min = stats_min(data, 5);
    double max = stats_max(data, 5);

    return (utils_double_equal(min, 1.0, 1e-6) && utils_double_equal(max, 9.0, 1e-6));
}

int test_stats_median(void)
{
    double data1[] = {1.0, 3.0, 5.0, 7.0, 9.0};
    double median1 = stats_median(data1, 5);

    double data2[] = {1.0, 2.0, 3.0, 4.0};
    double median2 = stats_median(data2, 4);

    return (utils_double_equal(median1, 5.0, 1e-6) && utils_double_equal(median2, 2.5, 1e-6));
}

int test_stats_variance(void)
{
    double data[] = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    double variance = stats_variance(data, 8);

    return utils_double_equal(variance, 4.0, 1e-6);
}

int run_stats_tests(void)
{
    int passed = 0;
    int total = 0;

    printf("Statistics Tests:\n");

    total++;
    if (test_stats_mean()) {
        printf("  [PASS] stats_mean\n");
        passed++;
    } else {
        printf("  [FAIL] stats_mean\n");
    }

    total++;
    if (test_stats_min_max()) {
        printf("  [PASS] stats_min_max\n");
        passed++;
    } else {
        printf("  [FAIL] stats_min_max\n");
    }

    total++;
    if (test_stats_median()) {
        printf("  [PASS] stats_median\n");
        passed++;
    } else {
        printf("  [FAIL] stats_median\n");
    }

    total++;
    if (test_stats_variance()) {
        printf("  [PASS] stats_variance\n");
        passed++;
    } else {
        printf("  [FAIL] stats_variance\n");
    }

    printf("  Stats: %d/%d tests passed\n\n", passed, total);

    return passed == total;
}
