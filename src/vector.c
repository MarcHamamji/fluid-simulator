#include "vector.h"
#include <math.h>

double vector_norm_squared(Vector a) { return a.x * a.x + a.y * a.y; }

double vector_norm(Vector a) { return sqrt(vector_norm_squared(a)); }

double vector_distance_squared(Vector a, Vector b) {
  Vector c = vector_subtract(a, b);
  return vector_norm_squared(c);
}

double vector_distance(Vector a, Vector b) {
  return sqrt(vector_distance_squared(a, b));
}

Vector vector_add(Vector a, Vector b) { return (Vector){a.x + b.x, a.y + b.y}; }

Vector vector_subtract(Vector a, Vector b) {
  return vector_add(a, vector_multiply(b, -1));
}

Vector vector_multiply(Vector a, double b) {
  return (Vector){a.x * b, a.y * b};
}

Vector vector_normalize(Vector a) {
  double n = vector_norm(a);
  return vector_multiply(a, 1 / n);
}

double vector_dot(Vector a, Vector b) {
  return a.x * b.x + a.y * b.y;
}
