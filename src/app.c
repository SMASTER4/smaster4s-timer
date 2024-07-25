#include <gtk/gtk.h>

#include "app.h"

extern int app_run(int argc, char** argv) {
  GtkApplication* app = gtk_application_new("de.smaster4.smaster4s-timer", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(_SIGNAL_app_activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}

static void _SIGNAL_app_activate(GtkApplication* app, gpointer user_data) {
  GtkWidget* window = gtk_application_window_new(app);

  gtk_window_set_title(GTK_WINDOW(window), "S-Master4's Timer");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  gtk_window_present(GTK_WINDOW(window));
}
