#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct {
    double *data;
    size_t rows;
    size_t cols;
} Matrix;

/* Matrix creation and destruction */
Matrix *matrix_create(size_t rows, size_t cols);
void matrix_free(Matrix *m);

/* Matrix operations */
Matrix *matrix_multiply(const Matrix *a, const Matrix *b);
Matrix *matrix_add(const Matrix *a, const Matrix *b);
Matrix *matrix_transpose(const Matrix *m);
void matrix_scale(Matrix *m, const double scalar);

/* Utility functions */
void matrix_print(const Matrix *m);
void matrix_fill_random(Matrix *m);
int matrix_equal(const Matrix *a, const Matrix *b, const double epsilon);

#endif /* MATRIX_H */
