#pragma once

#include <stdbool.h>
typedef struct {
  float x;
  float y;
} Vector;

#define VECTOR_ZERO (Vector) {0, 0}

Vector vector_new(float x, float y);

double vector_norm_squared(Vector a);

double vector_norm(Vector a);

double vector_distance_squared(Vector a, Vector b);

double vector_distance(Vector a, Vector b);

Vector vector_add(Vector a, Vector b);

Vector vector_subtract(Vector a, Vector b);

Vector vector_scale(Vector a, double b);

Vector vector_normalize(Vector a);

double vector_dot(Vector a, Vector b);

Vector vector_random();

Vector vector_random_in_rect(Vector rect);

Vector vector_divide(Vector a, Vector b);

Vector vector_floor(Vector a);

Vector vector_ceil(Vector a);

bool vector_is_in_rect(Vector a, Vector rect);
