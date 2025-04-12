#include <cairo.h>
#include <gtk/gtk.h>

#include "../utils/color.h"
#include "../utils/vector.h"
#include "particle.h"

Particle particle_generate(State *state, unsigned int radius,
                           float velocity_norm, Vector acceleration) {
  Vector velocity = vector_scale(vector_random(), velocity_norm);

  return (Particle){
      .radius = radius,
      .position = vector_random_in_rect(state->window_size),
      .velocity = velocity,
      .acceleration = acceleration,
      .state = state,
  };
}

void particle_draw(Particle *particle, cairo_t *cr) {
  // float velocity_norm = vector_norm(particle->velocity);
  // float hue_when_velocity_0 = 120;
  // float velocity_when_hue_0 = 15;
  // float hue = hue_when_velocity_0 -
  //             (hue_when_velocity_0 / velocity_when_hue_0) * velocity_norm;
  // Color color = color_new_from_hsv(hue, 1, 0.6);

  float hue = particle_get_density(particle) * 50000;
  Color color = color_new_from_hsv(hue, 1, 0.6);

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
    particle->velocity.x = -particle->velocity.x * particle->state->collision_damping;
  } else if (particle->position.x > max_x) {
    particle->position.x = max_x;
    particle->velocity.x = -particle->velocity.x * particle->state->collision_damping;
  }

  double min_y = particle->radius;
  double max_y = particle->state->window_size.y - particle->radius;

  if (particle->position.y < min_y) {
    particle->position.y = min_y;
    particle->velocity.y = -particle->velocity.y * particle->state->collision_damping;
  } else if (particle->position.y > max_y) {
    particle->position.y = max_y;
    particle->velocity.y = -particle->velocity.y * particle->state->collision_damping;
  }
}

float particle_get_density(Particle *particle) {
    float density = 0;
    State *state = particle->state;

    for (int i = 0; i < state->particles_number; i++) {
        Particle other = state->particles[i];
        float distance = vector_distance(particle->position, other.position);
        if (distance >= state->smoothing_radius) continue;

        float volume = (G_PI * pow(state->smoothing_radius, 5)) / 10;
        float influence = pow(state->smoothing_radius - distance, 3) / volume;
        density += influence;
    }

    return density;
}
