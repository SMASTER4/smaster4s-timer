#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include "gui.h"
#include "common.h"

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
  char* layout_full_path = get_config_path("/main.ui");
  gtk_builder_add_from_file(builder, layout_full_path, NULL);

  GObject* window = gtk_builder_get_object(builder, "window");
  if(window == NULL) {
    g_error("The window object inside ~%s couldn't be found!", layout_full_path);
    free(layout_full_path);
    return;
  }
  free(layout_full_path);
  gtk_window_set_application(GTK_WINDOW(window), app);

  GObject* timer_entry = gtk_builder_get_object(builder, "timer_entry");
  struct timer_entry_state* timer_entry_state;
  if(timer_entry != NULL) {
    timer_entry_state = g_malloc(sizeof(struct timer_entry_state));
    timer_entry_state->timer_entry = GTK_ENTRY(timer_entry);
    timer_entry_state->timer_entry_update_tag = 0;
    g_signal_connect(GTK_ENTRY(timer_entry), "activate", G_CALLBACK(_SIGNAL_timer_entry_activate), timer_entry_state);
  }

  GObject* timer_button = gtk_builder_get_object(builder, "timer_button");
  if(timer_button != NULL)
    if(timer_entry != NULL)
      g_signal_connect(GTK_BUTTON(timer_button), "clicked", G_CALLBACK(_SIGNAL_timer_button_clicked), timer_entry_state);
    else
      g_warning("You can't use the timer_button without a time_entry");

  gtk_window_present(GTK_WINDOW(window));
  g_object_unref(builder);
}

static void _SIGNAL_timer_entry_activate(GtkEntry* timer_entry, gpointer user_data) {
  _timer_toggle(user_data);
}

static void _SIGNAL_timer_button_clicked(GtkButton* timer_button, gpointer user_data) {
  _timer_toggle(user_data);
}

static void _timer_toggle(struct timer_entry_state* timer_entry_state) {
  if(timer_entry_state == NULL)
    return;

  if(timer_entry_state->timer_entry_update_tag != 0) {
    g_source_remove(timer_entry_state->timer_entry_update_tag);
    timer_entry_state->timer_entry_update_tag = 0;
    return;
  }

  if(timer_entry_state->timer_entry == NULL)
    return;

  timer_entry_state->timer_entry_update_tag = g_timeout_add(1000, (GSourceFunc) _timer_entry_update, timer_entry_state);
}

static gboolean _timer_entry_update(struct timer_entry_state* timer_entry_state) {
  if(timer_entry_state == NULL || timer_entry_state->timer_entry == NULL) {
    timer_entry_state->timer_entry_update_tag = 0;
    return FALSE;
  }
  GtkEntryBuffer* timer_entry_buffer = gtk_entry_get_buffer(timer_entry_state->timer_entry);
  int parsed_delay_buffer[3];
  parse_delay(gtk_entry_buffer_get_text(timer_entry_buffer), parsed_delay_buffer);
  parsed_delay_buffer[2]--;
  for(size_t i = 2; i > 0; i--) {
    if(parsed_delay_buffer[i] < 0) {
      parsed_delay_buffer[i] += 60;
      parsed_delay_buffer[i - 1] -= floor(parsed_delay_buffer[i] / 60) + 1;
    }
  }
  if(parsed_delay_buffer[0] < 0) {
    timer_entry_state->timer_entry_update_tag = 0;
    return FALSE;
  }
  size_t updated_delay_formated_length = get_lenght_as_string(parsed_delay_buffer[0]) + get_lenght_as_string(parsed_delay_buffer[1]) + get_lenght_as_string(parsed_delay_buffer[2]) + 2 + 1;
  char* updated_delay_formated = g_malloc(updated_delay_formated_length);
  snprintf(updated_delay_formated, updated_delay_formated_length, "%d:%d:%d", parsed_delay_buffer[0], parsed_delay_buffer[1], parsed_delay_buffer[2]);
  gtk_entry_buffer_set_text(timer_entry_buffer, updated_delay_formated, strlen(updated_delay_formated));
  g_free(updated_delay_formated);
  return TRUE;
}
