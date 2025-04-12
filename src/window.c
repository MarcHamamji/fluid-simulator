#include "window.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  Window *window;
  char *name;
  unsigned int width;
  unsigned int height;
  void (*draw)(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height,
               gpointer _data);
} WindowActivationParams;

static void activate(GtkApplication *app, WindowActivationParams *params) {
  Window *window = params->window;

  window->app_window = GTK_APPLICATION_WINDOW(gtk_application_window_new(app));

  gtk_window_set_title(GTK_WINDOW(window->app_window), params->name);
  gtk_window_set_default_size(GTK_WINDOW(window->app_window), params->width,
                              params->height + 36);
  gtk_window_set_resizable(GTK_WINDOW(window->app_window), false);

  window->drawing_area = GTK_DRAWING_AREA(gtk_drawing_area_new());
  gtk_window_set_child(GTK_WINDOW(window->app_window),
                       GTK_WIDGET(window->drawing_area));

  gtk_drawing_area_set_draw_func(window->drawing_area, params->draw, NULL,
                                 NULL);

  gtk_window_present(GTK_WINDOW(window->app_window));

  free(params);
}

Window *window_new(char *name, unsigned int width, unsigned int height,
                   void (*draw)(GtkDrawingArea *drawing_area, cairo_t *cr,
                                int width, int height, gpointer _data)) {

  WindowActivationParams *params = malloc(sizeof(WindowActivationParams));
  *params = (WindowActivationParams){
      .window = malloc(sizeof(Window)),
      .name = name,
      .width = width,
      .height = height,
      .draw = draw,
  };

  Window *window = params->window;

  window->app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(window->app, "activate", G_CALLBACK(activate),
                   (gpointer)params);

  window->width = width;
  window->height = height;

  return window;
}

int window_present(Window *window) {
  int status = g_application_run(G_APPLICATION(window->app), 0, NULL);
  return status;
}
