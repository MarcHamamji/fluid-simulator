#pragma once

#include "gtk/gtk.h"
#include "state.h"

void fluid_simulator_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width,
                          int height, double delta_time, State *state);

State *fluid_simulator_setup();
