#pragma once

#include <gtk/gtk.h>

typedef struct {
  unsigned int width;
  unsigned int height;
  GtkApplication *app;
  GtkApplicationWindow *app_window;
  GtkDrawingArea *drawing_area;
} Window;

Window *window_new(char *name, unsigned int width, unsigned int height,
                   void (*draw)(GtkDrawingArea *drawing_area, cairo_t *cr,
                                int width, int height, gpointer _data));

int window_present(Window *window);

void window_free(Window *window);
