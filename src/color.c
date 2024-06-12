#include "color.h"

static double hue_to_rgb(double p, double q, double t) {
  if (t < 0)
    t += 1;
  if (t > 1)
    t -= 1;
  if (t < 1.0 / 6.0)
    return p + (q - p) * 6 * t;
  if (t < 1.0 / 2.0)
    return q;
  if (t < 2.0 / 3.0)
    return p + (q - p) * (2.0 / 3.0 - t) * 6;
  return p;
}

Color color_new_from_hsl(double h, double s, double l) {
  Color c;
  if (s == 0) {
    c.red = l;
    c.green = l;
    c.blue = l;
  } else {
    double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    double p = 2 * l - q;
    c.red = hue_to_rgb(p, q, h + 1.0 / 3.0);
    c.green = hue_to_rgb(p, q, h);
    c.blue = hue_to_rgb(p, q, h - 1.0 / 3.0);
  }
  return c;
}
