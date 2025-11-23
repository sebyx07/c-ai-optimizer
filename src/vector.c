#include "vector.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Vector* vector_create(size_t size) {
    Vector *v = (Vector*)malloc(sizeof(Vector));
    if (v == NULL) {
        return NULL;
    }

    v->size = size;
    v->data = (double*)calloc(size, sizeof(double));

    if (v->data == NULL) {
        free(v);
        return NULL;
    }

    return v;
}

void vector_free(Vector *v) {
    if (v != NULL) {
        if (v->data != NULL) {
            free(v->data);
        }
        free(v);
    }
}

double vector_dot(const Vector *a, const Vector *b) {
    if (a == NULL || b == NULL || a->size != b->size) {
        return 0.0;
    }

    double result = 0.0;
    for (size_t i = 0; i < a->size; i++) {
        result += a->data[i] * b->data[i];
    }

    return result;
}

Vector* vector_add(const Vector *a, const Vector *b) {
    if (a == NULL || b == NULL || a->size != b->size) {
        return NULL;
    }

    Vector *result = vector_create(a->size);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < a->size; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }

    return result;
}

Vector* vector_scale(const Vector *v, double scalar) {
    if (v == NULL) {
        return NULL;
    }

    Vector *result = vector_create(v->size);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < v->size; i++) {
        result->data[i] = v->data[i] * scalar;
    }

    return result;
}

double vector_magnitude(const Vector *v) {
    if (v == NULL) {
        return 0.0;
    }

    double sum = 0.0;
    for (size_t i = 0; i < v->size; i++) {
        sum += v->data[i] * v->data[i];
    }

    return sqrt(sum);
}

Vector* vector_normalize(const Vector *v) {
    if (v == NULL) {
        return NULL;
    }

    double mag = vector_magnitude(v);
    if (mag < 1e-10) {
        return NULL;
    }

    return vector_scale(v, 1.0 / mag);
}

void vector_print(const Vector *v) {
    if (v == NULL) {
        printf("NULL vector\n");
        return;
    }

    printf("Vector (%zu): [", v->size);
    for (size_t i = 0; i < v->size; i++) {
        printf("%.3f", v->data[i]);
        if (i < v->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void vector_fill_random(Vector *v) {
    if (v == NULL) {
        return;
    }

    for (size_t i = 0; i < v->size; i++) {
        v->data[i] = utils_random_double(-10.0, 10.0);
    }
}

int vector_equal(const Vector *a, const Vector *b, double epsilon) {
    if (a == NULL || b == NULL) {
        return 0;
    }

    if (a->size != b->size) {
        return 0;
    }

    for (size_t i = 0; i < a->size; i++) {
        if (!utils_double_equal(a->data[i], b->data[i], epsilon)) {
            return 0;
        }
    }

    return 1;
}
