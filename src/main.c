#include <gtk/gtk.h>

#include "fluid_simulator/fluid_simulator.h"
#include "fluid_simulator/particle.h"
#include "fluid_simulator/state.h"
#include "utils/timer.h"
#include "utils/vector.h"
#include "window.h"

#define FRAMERATE 120

Timer timer;

Window *window;
State *state;

static bool trigger_draw(GtkDrawingArea *drawing_area) {
  gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
  return false;
}

static void draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width,
                 int height, gpointer _data) {

  timer_stop(&timer);
  double delta_time = timer_get_seconds(&timer);

  fluid_simulator_draw(drawing_area, cr, width, height, delta_time, state);

  char str[32];
  sprintf(str, "FPS: %d", (int)(1 / delta_time));
  cairo_set_source_rgb(cr, 255, 255, 255);
  cairo_move_to(cr, 16, 32);
  cairo_set_font_size(cr, 18);
  cairo_show_text(cr, str);

  timer_start(&timer);
  g_timeout_add(1000 / FRAMERATE, (GSourceFunc)trigger_draw, drawing_area);
}

static State *setup() {
  state = fluid_simulator_setup();
  return state;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));

  state = setup();
  window = window_new("Fluid Simulator", state->window_size.x,
                      state->window_size.y, draw),

  timer_start(&timer);
  int status = window_present(window);

  state_free(state);
  return status;
}
