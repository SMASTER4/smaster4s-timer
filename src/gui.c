#include <gtk/gtk.h>

#include "gui.h"

static const char* _layout_relative_path = "/.config/smaster4s-timer/main.ui";

extern int gui_run(int argc, char** argv) {
  g_print("Application launches in GUI mode.\n");
  GtkApplication* app = gtk_application_new("de.smaster4.smaster4s-timer", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(_SIGNAL_app_activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}

static void _SIGNAL_app_activate(GtkApplication* app, gpointer user_data) {
  GtkBuilder* builder = gtk_builder_new();
  {
    const char* home_path = getenv("HOME");
    if(home_path == NULL) {
      g_error("You have no HOME enviorement variable. Please define HOME to be your home directory.");
      return;
    }
    char layout_full_path[strlen(home_path) + strlen(_layout_relative_path) + 1];
    sprintf(layout_full_path, "%s%s", home_path, _layout_relative_path);
    gtk_builder_add_from_file(builder, layout_full_path, NULL);
  }

  GObject* window = gtk_builder_get_object(builder, "window");
  if(window == NULL) {
    g_error("The window object inside ~%s couldn't be found!", _layout_relative_path);
    return;
  }
  gtk_window_set_application(GTK_WINDOW(window), app);

  gtk_window_present(GTK_WINDOW(window));
}
