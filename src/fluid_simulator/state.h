#pragma once

#include <gtk/gtk.h>

#include "particle.h"

typedef struct State {
  Vector window_size; 

  unsigned int particles_number;
  Particle* particles;

  float collision_damping;
  float smoothing_radius;
} State;

void state_free(State *state);
