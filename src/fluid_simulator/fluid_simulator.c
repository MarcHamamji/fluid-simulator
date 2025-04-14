#include "fluid_simulator.h"
#include "gtk/gtk.h"
#include <stdlib.h>

#define WINDOW_SIZE vector_new(800, 600)

#define SPEED 200

#define PARTICLES_NUMBER 2048
#define PARTICLE_RADIUS 5

#define COLLISION_DAMPING 0.9
#define SMOOTHING_RADIUS 64

#define INITIAL_VELOCITY 1
#define ACCELERATION vector_new(0, 0.00)

void fluid_simulator_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width,
                          int height, double delta_time, State *state) {

  space_partitioning_grid_clear_cells(&state->space_partitioning_grid);

  int particles_number = state->particles_number;

  for (int i = 0; i < particles_number; i++) {
    Particle *particle = state->particles[i];
    particle_update_velocity(particle, delta_time * SPEED);
    particle_update_position(particle, delta_time * SPEED);
    particle_handle_wall_collisions(particle);

    space_partitioning_grid_add_particle(&state->space_partitioning_grid,
                                         particle);
  }

  for (int i = 0; i < particles_number; i++) {
    Particle *particle = state->particles[i];
    particle_draw(particle, cr);
  }
}

State *fluid_simulator_setup() {
  Particle **particles = calloc(PARTICLES_NUMBER, sizeof(Particle *));

  State *state = malloc(sizeof(State));
  *state = (State){
      .window_size = WINDOW_SIZE,
      .particles_number = PARTICLES_NUMBER,
      .particles = particles,
      .collision_damping = COLLISION_DAMPING,
      .smoothing_radius = SMOOTHING_RADIUS,
      .space_partitioning_grid = space_partitioning_grid_new(
          vector_new(SMOOTHING_RADIUS, SMOOTHING_RADIUS), WINDOW_SIZE),
  };

  for (int i = 0; i < PARTICLES_NUMBER; i++) {
    particles[i] = particle_generate(state, PARTICLE_RADIUS, INITIAL_VELOCITY,
                                     ACCELERATION);
    space_partitioning_grid_add_particle(&state->space_partitioning_grid,
                                         particles[i]);
  }

  return state;
}
