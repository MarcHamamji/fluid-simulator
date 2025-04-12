#include "fluid_simulator.h"
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

#define SPEED 50

#define PARTICLES_NUMBER 1024
#define PARTICLE_RADIUS 5

#define COLLISION_DAMPING 0.9
#define SMOOTHING_RADIUS 64

#define INITIAL_VELOCITY 6
#define ACCELERATION vector_new(0, 0)

void fluid_simulator_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width,
                          int height, double delta_time, State *state) {

  for (int i = 0; i < state->particles_number; i++) {
    Particle *particle = &state->particles[i];
    particle_update_velocity(particle, delta_time * SPEED);
    particle_update_position(particle, delta_time * SPEED);
    particle_handle_wall_collisions(particle);
    particle_draw(particle, cr);
  }
}

State *fluid_simulator_setup() {
  Particle *particles = calloc(PARTICLES_NUMBER, sizeof(Particle));

  State *state = malloc(sizeof(State));
  *state = (State){
      .window_size = vector_new(WIDTH, HEIGHT),
      .particles_number = PARTICLES_NUMBER,
      .particles = particles,
      .collision_damping = COLLISION_DAMPING,
      .smoothing_radius = SMOOTHING_RADIUS,
  };

  for (int i = 0; i < PARTICLES_NUMBER; i++) {
    particles[i] = particle_generate(state, PARTICLE_RADIUS, INITIAL_VELOCITY, ACCELERATION);
  }

  return state;
}
