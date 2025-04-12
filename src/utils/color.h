#pragma once

typedef struct {
  double red;
  double green;
  double blue;
} Color;

Color color_new_from_hsv(double h, double s, double v);
