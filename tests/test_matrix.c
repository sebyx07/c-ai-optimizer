#include "../include/matrix.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

int test_matrix_create(void)
{
    Matrix *m = matrix_create(3, 4);
    if (m == NULL) {
        return 0;
    }

    if (m->rows != 3 || m->cols != 4) {
        matrix_free(m);
        return 0;
    }

    matrix_free(m);
    return 1;
}

int test_matrix_add(void)
{
    Matrix *a = matrix_create(2, 2);
    Matrix *b = matrix_create(2, 2);

    a->data[0] = 1.0;
    a->data[1] = 2.0;
    a->data[2] = 3.0;
    a->data[3] = 4.0;

    b->data[0] = 5.0;
    b->data[1] = 6.0;
    b->data[2] = 7.0;
    b->data[3] = 8.0;

    Matrix *sum = matrix_add(a, b);

    int result = (sum->data[0] == 6.0 && sum->data[1] == 8.0 && sum->data[2] == 10.0 &&
                  sum->data[3] == 12.0);

    matrix_free(a);
    matrix_free(b);
    matrix_free(sum);

    return result;
}

int test_matrix_multiply(void)
{
    Matrix *a = matrix_create(2, 2);
    Matrix *b = matrix_create(2, 2);

    a->data[0] = 1.0;
    a->data[1] = 2.0;
    a->data[2] = 3.0;
    a->data[3] = 4.0;

    b->data[0] = 5.0;
    b->data[1] = 6.0;
    b->data[2] = 7.0;
    b->data[3] = 8.0;

    Matrix *product = matrix_multiply(a, b);

    int result = (product->data[0] == 19.0 && product->data[1] == 22.0 &&
                  product->data[2] == 43.0 && product->data[3] == 50.0);

    matrix_free(a);
    matrix_free(b);
    matrix_free(product);

    return result;
}

int test_matrix_transpose(void)
{
    Matrix *m = matrix_create(2, 3);

    m->data[0] = 1.0;
    m->data[1] = 2.0;
    m->data[2] = 3.0;
    m->data[3] = 4.0;
    m->data[4] = 5.0;
    m->data[5] = 6.0;

    Matrix *t = matrix_transpose(m);

    int result = (t->rows == 3 && t->cols == 2 && t->data[0] == 1.0 && t->data[1] == 4.0 &&
                  t->data[2] == 2.0 && t->data[3] == 5.0 && t->data[4] == 3.0 && t->data[5] == 6.0);

    matrix_free(m);
    matrix_free(t);

    return result;
}

int run_matrix_tests(void)
{
    int passed = 0;
    int total = 0;

    printf("Matrix Tests:\n");

    total++;
    if (test_matrix_create()) {
        printf("  [PASS] matrix_create\n");
        passed++;
    } else {
        printf("  [FAIL] matrix_create\n");
    }

    total++;
    if (test_matrix_add()) {
        printf("  [PASS] matrix_add\n");
        passed++;
    } else {
        printf("  [FAIL] matrix_add\n");
    }

    total++;
    if (test_matrix_multiply()) {
        printf("  [PASS] matrix_multiply\n");
        passed++;
    } else {
        printf("  [FAIL] matrix_multiply\n");
    }

    total++;
    if (test_matrix_transpose()) {
        printf("  [PASS] matrix_transpose\n");
        passed++;
    } else {
        printf("  [FAIL] matrix_transpose\n");
    }

    printf("  Matrix: %d/%d tests passed\n\n", passed, total);

    return passed == total;
}
