#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
    double *data;
    size_t size;
} Vector;

/* Vector creation and destruction */
Vector* vector_create(size_t size);
void vector_free(Vector *v);

/* Vector operations */
double vector_dot(const Vector *a, const Vector *b);
Vector* vector_add(const Vector *a, const Vector *b);
Vector* vector_scale(const Vector *v, double scalar);
double vector_magnitude(const Vector *v);
Vector* vector_normalize(const Vector *v);

/* Utility functions */
void vector_print(const Vector *v);
void vector_fill_random(Vector *v);
int vector_equal(const Vector *a, const Vector *b, double epsilon);

#endif /* VECTOR_H */
