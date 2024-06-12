#include <cairo.h>
#include <gtk/gtk.h>

#include "particle.h"
#include "vector.h"

static Particle particle_generate(unsigned int width, unsigned int height,
                                  unsigned int radius, float velocity_norm,
                                  Vector acceleration) {
  Vector velocity = (Vector){rand() % 10 - 5, rand() % 10 - 5};
  velocity = vector_multiply(vector_normalize(velocity), velocity_norm);

  return (Particle){
      .radius = radius,
      .position = (Vector){rand() % width, rand() % height},
      .velocity = velocity,
      .acceleration = acceleration,
      .color = color_new_from_hsl((double)rand() / RAND_MAX, 0.2, 0.5)};
}

Particle *particles_generate_without_intersections(
    unsigned int n, unsigned int width, unsigned int height,
    unsigned int radius, float velocity_norm, Vector acceleration) {

  Particle *particles = malloc(sizeof(Particle) * n);
  for (int i = 0; i < n; i++) {
    while (true) {
      particles[i] =
          particle_generate(width, height, radius, velocity_norm, acceleration);
      bool intersection = false;
      for (int j = 0; j < i; j++) {
        if (particle_is_colliding(&particles[i], &particles[j])) {
          intersection = true;
          break;
        }
      }
      if (!intersection) {
        break;
      }
    }
  }

  return particles;
}

void particle_draw(Particle *particle, cairo_t *cr) {
  cairo_set_source_rgb(cr, particle->color.red, particle->color.green,
                       particle->color.blue);
  cairo_arc(cr, particle->position.x, particle->position.y, particle->radius, 0,
            2 * G_PI);
  cairo_fill(cr);
}

void particle_update_state(Particle *particle, double dt) {
  particle->velocity.x += particle->acceleration.x * dt;
  particle->velocity.y += particle->acceleration.y * dt;
  particle->position.x += particle->velocity.x * dt;
  particle->position.y += particle->velocity.y * dt;

  double loss_factor = pow(0.99, dt);
  particle->velocity = vector_multiply(particle->velocity, loss_factor);
}

void particle_handle_wall_collisions(Particle *particle, unsigned int width,
                                     unsigned int height) {
  double min_x = particle->radius;
  double max_x = width - particle->radius;
  double min_y = particle->radius;
  double max_y = height - particle->radius;

  if (particle->position.x < min_x && particle->velocity.x < 0) {
    particle->position.x = 2 * min_x - particle->position.x;
    particle->velocity.x = -particle->velocity.x;
  } else if (particle->position.x > max_x && particle->velocity.x > 0) {
    particle->position.x = 2 * max_x - particle->position.x;
    particle->velocity.x = -particle->velocity.x;
  } else if (particle->position.y < min_y && particle->velocity.y < 0) {
    particle->position.y = 2 * min_y - particle->position.y;
    particle->velocity.y = -particle->velocity.y;
  } else if (particle->position.y > max_y && particle->velocity.y > 0) {
    particle->position.y = 2 * max_y - particle->position.y;
    particle->velocity.y = -particle->velocity.y;
  }
}

void particle_update(Particle *particle, State *state, double dt, cairo_t *cr) {
  particle_update_state(particle, dt);
  particle_handle_wall_collisions(particle, state->window->width,
                                  state->window->height);
  particle_draw(particle, cr);
}

bool particle_is_colliding(Particle *p1, Particle *p2) {

  if (fabsf(p1->position.x - p2->position.x) > p1->radius + p2->radius ||
      fabsf(p1->position.y - p2->position.y) > p1->radius + p2->radius) {
    return false;
  }

  double distance_squared = vector_distance_squared(p1->position, p2->position);
  bool too_close =
      distance_squared <= (p1->radius + p2->radius) * (p1->radius + p2->radius);

  bool approaching =
      vector_dot(vector_subtract(p1->velocity, p2->velocity),
                 vector_subtract(p1->position, p2->position)) < 0;

  return too_close && approaching;
}

void particle_collide(Particle *p1, Particle *p2) {
  Vector delta = vector_subtract(p1->position, p2->position);
  Vector normal = vector_normalize(delta);
  p1->velocity = vector_subtract(
      p1->velocity,
      vector_multiply(normal, 2 * vector_dot(p1->velocity, normal)));
  p2->velocity = vector_subtract(
      p2->velocity,
      vector_multiply(normal, 2 * vector_dot(p2->velocity, normal)));
}
