#include <gtk/gtk.h>

#include "particle.h"
#include "state.h"
#include "timer.h"
#include "vector.h"
#include "window.h"

#define WIDTH 800
#define HEIGHT 600
#define FRAMERATE 60

#define SPEED 1

Timer timer;
State state;

static bool trigger_draw(GtkDrawingArea *drawing_area) {
  gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
  return false;
}

static void draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width,
                 int height, gpointer user_data) {

  timer_stop(&timer);
  double delta_time = timer_get_seconds(&timer) * SPEED;

  for (int i = 0; i < state.particles_number; i++) {
    particle_update(&state.particles[i], &state, delta_time, cr);
    for (int j = i + 1; j < state.particles_number; j++) {
      if (particle_is_colliding(&state.particles[i], &state.particles[j])) {
        particle_collide(&state.particles[i], &state.particles[j]);
      }
    }
  }

  timer_start(&timer);
  g_timeout_add(1000 / FRAMERATE, (GSourceFunc)trigger_draw, drawing_area);
}

static gboolean on_key_press(GtkEventControllerKey *controller, guint keyval,
                             guint keycode, GdkModifierType modifier) {
  return true;
}

void on_drag_start(GtkGestureDrag *gesture, gdouble start_x, gdouble start_y,
                   gpointer user_data) {
  Vector cursor = (Vector){start_x, start_y};
  for (int i = 0; i < state.particles_number; i++) {
    Vector direction = vector_subtract(state.particles[i].position, cursor);
    double distance = vector_norm(direction);
    if (distance < 100) {
      state.particles[i].velocity = vector_add(
          state.particles[i].velocity,
          vector_multiply(vector_normalize(direction), 10000 / (distance + 1)));
    }
  }
}

void on_drag_update(GtkGestureDrag *gesture, gdouble offset_x, gdouble offset_y,
                    gpointer user_data) {}

int main(int argc, char *argv[]) {

  srand(time(NULL));

  const unsigned int particles_number = 1000;
  Particle *particles = particles_generate_without_intersections(
      particles_number, WIDTH, HEIGHT, 4, 0, (Vector){0, 100});

  state = (State){
      .particles_number = particles_number,
      .particles = particles,
      .window = window_new("Fluid Simulator", WIDTH, HEIGHT, draw, on_key_press,
                           on_drag_start, on_drag_update),
  };

  timer_start(&timer);
  int status = window_present(state.window);

  g_free(state.particles);

  return status;
}
