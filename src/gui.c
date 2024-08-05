#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "gui.h"
#include "common.h"

#ifdef __linux__
static const char* _layout_relative_path = "/.config/smaster4s-timer/main.ui";
#endif
#ifdef _WIN32
static const char* _layout_relative_path = "/smaster4s-timer/main.ui";
#endif

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
  #ifdef __linux__
  const char* home_path = getenv("HOME");
  #endif
  #ifdef _WIN32
  const char* home_path = getenv("APPDATA");
  #endif
  if(home_path == NULL) {
    #ifdef __linux__
    g_error("You have no HOME enviorement variable. Please define HOME to be your home directory.");
    #endif
    #ifdef _WIN32
    g_error("You have no APPDATA enviorement variable. Please define APPDATA to be your app data folder. (It should mostly be C:\\Users\\!YOUR_USER_NAME!\\AppData\\Roaming\\)"); // btw folder != directory
    #endif
    return;
  }
  char* layout_full_path = g_malloc(strlen(home_path) + strlen(_layout_relative_path) + 1);
  sprintf(layout_full_path, "%s%s", home_path, _layout_relative_path);
  gtk_builder_add_from_file(builder, layout_full_path, NULL);
  g_free(layout_full_path);

  GObject* window = gtk_builder_get_object(builder, "window");
  if(window == NULL) {
    g_error("The window object inside ~%s couldn't be found!", _layout_relative_path);
    return;
  }
  gtk_window_set_application(GTK_WINDOW(window), app);
  GObject* timer_entry = gtk_builder_get_object(builder, "timer_entry");
  if(timer_entry != NULL)
    g_signal_connect(GTK_ENTRY(timer_entry), "activate", G_CALLBACK(_SIGNAL_timer_entry_activate), NULL);

  gtk_window_present(GTK_WINDOW(window));
  g_object_unref(builder);
}

static void _SIGNAL_timer_entry_activate(GtkEntry* timer_entry, gpointer user_data) {
  g_timeout_add(1000, (GSourceFunc)_timer_entry_update, timer_entry);
}

static gboolean _timer_entry_update(GtkEntry* timer_entry) {
  if(timer_entry == NULL)
    return FALSE;
  GtkEntryBuffer* timer_entry_buffer = gtk_entry_get_buffer(timer_entry);
  int parsed_delay_buffer[3];
  parse_delay(gtk_entry_buffer_get_text(timer_entry_buffer), parsed_delay_buffer);
  parsed_delay_buffer[2]--;
  for(size_t i = 2; i > 0; i--) {
    if(parsed_delay_buffer[i] < 0) {
      parsed_delay_buffer[i] += 60;
      parsed_delay_buffer[i - 1] -= floor(parsed_delay_buffer[i] / 60) + 1;
    }
  }
  if(parsed_delay_buffer[0] < 0)
    return FALSE;
  size_t updated_delay_formated_length = get_lenght_as_string(parsed_delay_buffer[0]) + get_lenght_as_string(parsed_delay_buffer[1]) + get_lenght_as_string(parsed_delay_buffer[2]) + 2 + 1;
  char* updated_delay_formated = g_malloc(updated_delay_formated_length);
  snprintf(updated_delay_formated, updated_delay_formated_length, "%d:%d:%d", parsed_delay_buffer[0], parsed_delay_buffer[1], parsed_delay_buffer[2]);
  gtk_entry_buffer_set_text(timer_entry_buffer, updated_delay_formated, strlen(updated_delay_formated));
  g_free(updated_delay_formated);
  return TRUE;
}
