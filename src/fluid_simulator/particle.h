#pragma once
#include <cairo.h>
#include <stdbool.h>

#include "../utils/vector.h"

typedef struct State State;

typedef struct {
  Vector position;
  Vector velocity;
  Vector acceleration;
  unsigned int radius;
  State *state;
} Particle;

#include "state.h"

Particle particle_generate(State *state, unsigned int radius,
                           float velocity_norm, Vector acceleration);

void particle_draw(Particle *particle, cairo_t *cr);

void particle_update_velocity(Particle *particle, double dt);

void particle_update_position(Particle *particle, double dt);

void particle_handle_wall_collisions(Particle *particle);

float particle_get_density(Particle *particle);
