#pragma once

#include "../utils/vector.h"
#include "space_partitioning_grid.h"
#include "particle.h"

typedef struct State {
  Vector window_size; 

  unsigned int particles_number;
  Particle** particles;
  SpacePartitioningGrid space_partitioning_grid;

  float particle_radius;
  float collision_damping;
  float smoothing_radius;
} State;

void state_free(State *state);
