#pragma once
#include <cairo.h>
#include <stdbool.h>

#include "color.h"
#include "vector.h"

typedef struct State State;

typedef struct {
  Vector position;
  Vector velocity;
  Vector acceleration;
  unsigned int radius;
  Color color;
} Particle;

#include "state.h"

Particle *particles_generate_without_intersections(
    unsigned int n, unsigned int width, unsigned int height,
    unsigned int radius, float velocity_norm, Vector acceleration);

void particle_draw(Particle *particle, cairo_t *cr);

void particle_update_state(Particle *particle, double dt);

void particle_handle_wall_collisions(Particle *particle, unsigned int width,
                                     unsigned int height);

void particle_update(Particle *particle, State *state, double dt, cairo_t *cr);

bool particle_is_colliding(Particle *p1, Particle *p2);

void particle_collide(Particle *p1, Particle *p2);
