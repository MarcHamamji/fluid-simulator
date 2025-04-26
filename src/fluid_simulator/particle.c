#include <cairo.h>
#include <gtk/gtk.h>

#include "../utils/color.h"
#include "../utils/vector.h"
#include "particle.h"
#include "space_partitioning_grid.h"
#include "state.h"

Particle *particle_generate(State *state, unsigned int radius,
                            float velocity_norm, Vector acceleration) {

  Vector random_direction = vector_random();
  Vector velocity = vector_scale(&random_direction, velocity_norm);

  Particle *particle = malloc(sizeof(Particle));
  *particle = (Particle){
      .radius = radius,
      .position = vector_random_in_rect(&state->window_size),
      .velocity = velocity,
      .acceleration = acceleration,
      .state = state,
  };

  return particle;
}

void particle_draw(Particle *particle, cairo_t *cr) {
  float hue = 220 - particle->density * 20000;
  Color color = color_new_from_hsv(hue < 0 ? 0 : hue, 1, 0.6);

  cairo_set_source_rgb(cr, color.red, color.green, color.blue);
  cairo_arc(cr, particle->position.x, particle->position.y, particle->radius, 0,
            2 * G_PI);
  cairo_fill(cr);
}

void particle_update_velocity(Particle *particle, double dt) {
  particle->velocity.x += particle->acceleration.x * dt;
  particle->velocity.y += particle->acceleration.y * dt;
}

void particle_update_position(Particle *particle, double dt) {
  particle->position.x += particle->velocity.x * dt;
  particle->position.y += particle->velocity.y * dt;
}

void particle_handle_wall_collisions(Particle *particle) {
  double min_x = particle->radius;
  double max_x = particle->state->window_size.x - particle->radius;

  if (particle->position.x < min_x) {
    particle->position.x = min_x;
    particle->velocity.x =
        -particle->velocity.x * particle->state->collision_damping;
  } else if (particle->position.x > max_x) {
    particle->position.x = max_x;
    particle->velocity.x =
        -particle->velocity.x * particle->state->collision_damping;
  }

  double min_y = particle->radius;
  double max_y = particle->state->window_size.y - particle->radius;

  if (particle->position.y < min_y) {
    particle->position.y = min_y;
    particle->velocity.y =
        -particle->velocity.y * particle->state->collision_damping;
  } else if (particle->position.y > max_y) {
    particle->position.y = max_y;
    particle->velocity.y =
        -particle->velocity.y * particle->state->collision_damping;
  }
}

static float density_function(Vector *source, Particle *neighbor,
                              State *state) {
  float distance_squared = vector_distance_squared(source, &neighbor->position);
  if (distance_squared >= powf(state->smoothing_radius, 2))
    return 0;

  float distance = sqrtf(distance_squared);
  float volume = (G_PI * powf(state->smoothing_radius, 5)) / 10;
  float influence = powf(state->smoothing_radius - distance, 3) / volume;

  return influence;
}

static float cached_density_function(Vector *source, Particle *neighbor,
                                     State *state) {
  return neighbor->density;
}

void particle_update_density(Particle *particle) {
  float density = space_partitioning_grid_accumulate_over_neighbors(
      &particle->state->space_partitioning_grid, &particle->position,
      particle->state, density_function);

  particle->density = density;
}

float point_get_density(Vector *source, State *state) {
  // TODO I SHOULD REUSE THE STORED DENSITY
  //
  // if (source->x >= state->window_size.x - state->particle_radius ||
  //     source->x <= state->particle_radius ||
  //     source->y >= state->window_size.y - state->particle_radius ||
  //     source->y <= state->particle_radius)
  //   return 10;

  float density = space_partitioning_grid_accumulate_over_neighbors(
      &state->space_partitioning_grid, source, state, density_function);
  return density;
}

Vector particle_get_gradient(Particle *particle) {
  // TODO handle the edge case when the particle is at the edge of the window
  // right, now, there are no particles outside, so the gradient points outside
  float delta = 0.001;
  Vector delta_vector_x = vector_new(delta, 0);
  Vector delta_vector_y = vector_new(0, delta);

  Vector positive_vector_x = vector_add(&particle->position, &delta_vector_x);
  Vector negative_vector_x =
      vector_subtract(&particle->position, &delta_vector_x);

  float gradient_x = (point_get_density(&positive_vector_x, particle->state) -
                      point_get_density(&negative_vector_x, particle->state)) /
                     (2 * delta);

  Vector positive_vector_y = vector_add(&particle->position, &delta_vector_y);
  Vector negative_vector_y =
      vector_subtract(&particle->position, &delta_vector_y);

  float gradient_y = (point_get_density(&positive_vector_y, particle->state) -
                      point_get_density(&negative_vector_y, particle->state)) /
                     (2 * delta);

  Vector gradient = vector_new(gradient_x, gradient_y);
  return gradient;
}
