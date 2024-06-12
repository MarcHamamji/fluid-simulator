#pragma once

#include <gtk/gtk.h>

#include "particle.h"
#include "window.h"

typedef struct State {
  unsigned int particles_number;
  Particle *particles;

  Window *window;
} State;
