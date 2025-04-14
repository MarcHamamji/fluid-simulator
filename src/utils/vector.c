#include "vector.h"
#include "glib.h"
#include <math.h>
#include <stdlib.h>

Vector vector_new(float x, float y) { return (Vector){x, y}; }

double vector_norm_squared(Vector *a) { return a->x * a->x + a->y * a->y; }

double vector_norm(Vector *a) { return sqrtf(vector_norm_squared(a)); }

double vector_distance_squared(Vector *a, Vector *b) {
  float delta_x = a->x - b->x;
  float delta_y = a->y - b->y;
  return delta_x * delta_x + delta_y * delta_y;
}

double vector_distance(Vector *a, Vector *b) {
  return sqrtf(vector_distance_squared(a, b));
}

Vector vector_add(Vector *a, Vector *b) {
  return (Vector){a->x + b->x, a->y + b->y};
}

Vector vector_subtract(Vector *a, Vector *b) {
  return vector_new(a->x - b->x, a->y - b->y);
}

Vector vector_scale(Vector *a, float b) { return (Vector){a->x * b, a->y * b}; }

Vector vector_normalize(Vector *a) {
  double n = vector_norm(a);
  return vector_scale(a, 1 / n);
}

double vector_dot(Vector *a, Vector *b) { return a->x * b->x + a->y * b->y; }

Vector vector_random() {
  float theta = (float)rand() / RAND_MAX * G_PI * 2;
  return vector_new(cos(theta), sin(theta));
}

Vector vector_random_in_rect(Vector *rect) {
  return vector_new(rand() % (int)rect->x, rand() % (int)rect->y);
}

Vector vector_divide(Vector *a, Vector *b) {
  return vector_new(a->x / b->x, a->y / b->y);
}

Vector vector_floor(Vector *a) { return vector_new(floor(a->x), floor(a->y)); }

Vector vector_ceil(Vector *a) { return vector_new(ceil(a->x), ceil(a->y)); }

bool vector_is_in_rect(Vector *a, Vector *rect) {
  return a->x >= 0 && a->x < rect->x && a->y >= 0 && a->y < rect->y;
}
