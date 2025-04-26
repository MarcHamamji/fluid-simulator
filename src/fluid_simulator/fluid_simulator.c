#include "fluid_simulator.h"
#include "gtk/gtk.h"
#include <stdlib.h>

#define WINDOW_SIZE vector_new(800, 600)

#define SPEED 200

#define PARTICLES_NUMBER 2048
#define PARTICLE_RADIUS 5

#define COLLISION_DAMPING 0.9
#define SMOOTHING_RADIUS 64

#define INITIAL_VELOCITY 0
#define ACCELERATION vector_new(0, 0.00)

static void draw_arrow(Vector *position, Vector *arrow, float scale,
                       cairo_t *cr) {
  cairo_save(cr);

  cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);

  // Move to starting position
  cairo_translate(cr, position->x, position->y);

  // Compute the arrow tip
  float tip_x = arrow->x * scale;
  float tip_y = arrow->y * scale;

  // Draw the main arrow shaft
  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, tip_x, tip_y);
  cairo_stroke(cr);

  // Draw the arrowhead
  float arrow_length = sqrtf(tip_x * tip_x + tip_y * tip_y);
  if (arrow_length > 0) {
    float head_size = 10.0f; // size of the arrowhead

    // Unit vector of the arrow direction
    float ux = tip_x / arrow_length;
    float uy = tip_y / arrow_length;

    // Perpendicular vector for the arrowhead
    float perp_x = -uy;
    float perp_y = ux;

    cairo_move_to(cr, tip_x, tip_y);
    cairo_line_to(cr, tip_x - head_size * (ux + 0.5f * perp_x),
                  tip_y - head_size * (uy + 0.5f * perp_y));
    cairo_line_to(cr, tip_x - head_size * (ux - 0.5f * perp_x),
                  tip_y - head_size * (uy - 0.5f * perp_y));
    cairo_close_path(cr);
    cairo_fill(cr);
  }

  cairo_restore(cr);
}

void fluid_simulator_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width,
                          int height, double delta_time, State *state) {

  space_partitioning_grid_clear_cells(&state->space_partitioning_grid);

  int particles_number = state->particles_number;

#pragma omp parallel for
  for (int i = 0; i < particles_number; i++) {
    Particle *particle = state->particles[i];
    particle_update_velocity(particle, delta_time * SPEED);
    particle_update_position(particle, delta_time * SPEED);
    particle_handle_wall_collisions(particle);
  }

  for (int i = 0; i < particles_number; i++) {
    Particle *particle = state->particles[i];
    space_partitioning_grid_add_particle(&state->space_partitioning_grid,
                                         particle);
  }

#pragma omp parallel for
  for (int i = 0; i < particles_number; i++) {
    Particle *particle = state->particles[i];
    particle_update_density(particle);
  }

#pragma omp parallel for
  for (int i = 0; i < particles_number; i++) {
    Particle *particle = state->particles[i];

    Vector gradient = particle_get_gradient(particle);
    // draw_arrow(&particle->position, &gradient, -400000, cr);

    // TODO SHOULD UPDATE THE ACCELERATION, NOT THE VELOCITY
    // but in this case, i would have to add action reaction
    particle->velocity = vector_scale(&gradient, -3000);
    // particle->acceleration = vector_scale(&gradient, -10);
  }

  for (int i = 0; i < particles_number; i++) {
    Particle *particle = state->particles[i];
    particle_draw(particle, cr);
  }

  if (state->window_size.x > 600) {
    state->window_size.x -= 3;
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
      .particle_radius = PARTICLE_RADIUS,
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
