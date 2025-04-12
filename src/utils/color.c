#include "color.h"
#include <math.h>

Color color_new_from_hsv(double h, double s, double v) {
  Color c;
  // Normalize the hue to be within [0, 360)
  h = fmod(h, 360.0);
  if (h < 0)
    h += 360.0;

  // Clamp saturation and value to [0, 1]
  s = s < 0 ? 0 : (s > 1 ? 1 : s);
  v = v < 0 ? 0 : (v > 1 ? 1 : v);

  if (s == 0) {
    // Achromatic (grey)
    c.red = c.green = c.blue = v; // All components are equal
  } else {
    double f, p, q, t;
    int i;

    h /= 60;    // Sector 0 to 5
    i = (int)h; // Integer part of h
    f = h - i;  // Fractional part of h
    p = v * (1 - s);
    q = v * (1 - f * s);
    t = v * (1 - (1 - f) * s);

    switch (i) {
    case 0:
      c.red = v;
      c.green = t;
      c.blue = p;
      break;
    case 1:
      c.red = q;
      c.green = v;
      c.blue = p;
      break;
    case 2:
      c.red = p;
      c.green = v;
      c.blue = t;
      break;
    case 3:
      c.red = p;
      c.green = q;
      c.blue = v;
      break;
    case 4:
      c.red = t;
      c.green = p;
      c.blue = v;
      break;
    case 5:
    default:
      c.red = v;
      c.green = p;
      c.blue = q;
      break;
    }
  }

  return c;
}
