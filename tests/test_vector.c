#include "../include/vector.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int test_vector_create(void) {
    Vector *v = vector_create(5);
    if (v == NULL) {
        return 0;
    }

    if (v->size != 5) {
        vector_free(v);
        return 0;
    }

    vector_free(v);
    return 1;
}

int test_vector_dot(void) {
    Vector *a = vector_create(3);
    Vector *b = vector_create(3);

    a->data[0] = 1.0; a->data[1] = 2.0; a->data[2] = 3.0;
    b->data[0] = 4.0; b->data[1] = 5.0; b->data[2] = 6.0;

    double dot = vector_dot(a, b);

    int result = utils_double_equal(dot, 32.0, 1e-6);

    vector_free(a);
    vector_free(b);

    return result;
}

int test_vector_add(void) {
    Vector *a = vector_create(3);
    Vector *b = vector_create(3);

    a->data[0] = 1.0; a->data[1] = 2.0; a->data[2] = 3.0;
    b->data[0] = 4.0; b->data[1] = 5.0; b->data[2] = 6.0;

    Vector *sum = vector_add(a, b);

    int result = (sum->data[0] == 5.0 && sum->data[1] == 7.0 && sum->data[2] == 9.0);

    vector_free(a);
    vector_free(b);
    vector_free(sum);

    return result;
}

int test_vector_magnitude(void) {
    Vector *v = vector_create(3);

    v->data[0] = 3.0;
    v->data[1] = 4.0;
    v->data[2] = 0.0;

    double mag = vector_magnitude(v);

    int result = utils_double_equal(mag, 5.0, 1e-6);

    vector_free(v);

    return result;
}

int run_vector_tests(void) {
    int passed = 0;
    int total = 0;

    printf("Vector Tests:\n");

    total++;
    if (test_vector_create()) {
        printf("  [PASS] vector_create\n");
        passed++;
    } else {
        printf("  [FAIL] vector_create\n");
    }

    total++;
    if (test_vector_dot()) {
        printf("  [PASS] vector_dot\n");
        passed++;
    } else {
        printf("  [FAIL] vector_dot\n");
    }

    total++;
    if (test_vector_add()) {
        printf("  [PASS] vector_add\n");
        passed++;
    } else {
        printf("  [FAIL] vector_add\n");
    }

    total++;
    if (test_vector_magnitude()) {
        printf("  [PASS] vector_magnitude\n");
        passed++;
    } else {
        printf("  [FAIL] vector_magnitude\n");
    }

    printf("  Vector: %d/%d tests passed\n\n", passed, total);

    return passed == total;
}
