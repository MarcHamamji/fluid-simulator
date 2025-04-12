#include "vector.h"
#include "glib.h"
#include <math.h>
#include <stdlib.h>

Vector vector_new(float x, float y) { return (Vector){x, y}; }

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
  return vector_add(a, vector_scale(b, -1));
}

Vector vector_scale(Vector a, double b) { return (Vector){a.x * b, a.y * b}; }

Vector vector_normalize(Vector a) {
  double n = vector_norm(a);
  return vector_scale(a, 1 / n);
}

double vector_dot(Vector a, Vector b) { return a.x * b.x + a.y * b.y; }

Vector vector_random() {
  float theta = (float)rand() / RAND_MAX * G_PI * 2;
  return vector_new(cos(theta), sin(theta));
}

Vector vector_random_in_rect(Vector bounds) {
  return vector_new(rand() % (int)bounds.x, rand() % (int)bounds.y);
}
