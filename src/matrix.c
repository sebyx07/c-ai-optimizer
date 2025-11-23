#include "matrix.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Matrix* matrix_create(size_t rows, size_t cols) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    if (m == NULL) {
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;
    m->data = (double*)calloc(rows * cols, sizeof(double));

    if (m->data == NULL) {
        free(m);
        return NULL;
    }

    return m;
}

void matrix_free(Matrix *m) {
    if (m != NULL) {
        if (m->data != NULL) {
            free(m->data);
        }
        free(m);
    }
}

Matrix* matrix_multiply(const Matrix *a, const Matrix *b) {
    if (a == NULL || b == NULL || a->cols != b->rows) {
        return NULL;
    }

    Matrix *result = matrix_create(a->rows, b->cols);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }

    return result;
}

Matrix* matrix_add(const Matrix *a, const Matrix *b) {
    if (a == NULL || b == NULL || a->rows != b->rows || a->cols != b->cols) {
        return NULL;
    }

    Matrix *result = matrix_create(a->rows, a->cols);
    if (result == NULL) {
        return NULL;
    }

    size_t total = a->rows * a->cols;
    for (size_t i = 0; i < total; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }

    return result;
}

Matrix* matrix_transpose(const Matrix *m) {
    if (m == NULL) {
        return NULL;
    }

    Matrix *result = matrix_create(m->cols, m->rows);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            result->data[j * result->cols + i] = m->data[i * m->cols + j];
        }
    }

    return result;
}

void matrix_scale(Matrix *m, double scalar) {
    if (m == NULL) {
        return;
    }

    size_t total = m->rows * m->cols;
    for (size_t i = 0; i < total; i++) {
        m->data[i] *= scalar;
    }
}

void matrix_print(const Matrix *m) {
    if (m == NULL) {
        printf("NULL matrix\n");
        return;
    }

    printf("Matrix (%zu x %zu):\n", m->rows, m->cols);
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.3f ", m->data[i * m->cols + j]);
        }
        printf("\n");
    }
}

void matrix_fill_random(Matrix *m) {
    if (m == NULL) {
        return;
    }

    size_t total = m->rows * m->cols;
    for (size_t i = 0; i < total; i++) {
        m->data[i] = utils_random_double(-10.0, 10.0);
    }
}

int matrix_equal(const Matrix *a, const Matrix *b, double epsilon) {
    if (a == NULL || b == NULL) {
        return 0;
    }

    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }

    size_t total = a->rows * a->cols;
    for (size_t i = 0; i < total; i++) {
        if (!utils_double_equal(a->data[i], b->data[i], epsilon)) {
            return 0;
        }
    }

    return 1;
}
