#pragma once

typedef struct {
  float x;
  float y;
} Vector;

double vector_norm_squared(Vector a);

double vector_norm(Vector a);

double vector_distance_squared(Vector a, Vector b);

double vector_distance(Vector a, Vector b);

Vector vector_add(Vector a, Vector b);

Vector vector_subtract(Vector a, Vector b);

Vector vector_multiply(Vector a, double b);

Vector vector_normalize(Vector a);

double vector_dot(Vector a, Vector b);

