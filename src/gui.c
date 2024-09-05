#include <gtk/gtk.h>
#include <smaster4s-inis.h>
#include <sys/stat.h>

#ifdef __unix__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <io.h>
#define access _access
#include "file_flags.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  _load_css();

  GtkBuilder* builder = gtk_builder_new();
  char* layout_full_path = get_config_path("main.ui");
  if(layout_full_path == NULL)
    return;
  gtk_builder_add_from_file(builder, layout_full_path, NULL);

  GObject* window = gtk_builder_get_object(builder, "window");
  if(window == NULL) {
    g_error("The window object inside %s couldn't be found!", layout_full_path);
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
    timer_entry_state->timer_button = NULL;
    timer_entry_state->timer_entry_update_tag = 0;
    g_signal_connect(GTK_ENTRY(timer_entry), "activate", G_CALLBACK(_SIGNAL_timer_entry_activate), timer_entry_state);
  }

  GObject* timer_button = gtk_builder_get_object(builder, "timer_button");
  if(timer_button != NULL)
    if(timer_entry != NULL) {
      timer_entry_state->timer_button = GTK_BUTTON(timer_button);
      g_signal_connect(GTK_BUTTON(timer_button), "clicked", G_CALLBACK(_SIGNAL_timer_button_clicked), timer_entry_state);
    }
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


static void _load_css() {
  char* css_path = get_config_path("main.css");
  if(css_path == NULL)
    return;

  GError** error;

  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, css_path);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  g_object_unref(provider);
  free(css_path);
}


static void _timer_toggle(struct timer_entry_state* timer_entry_state) {
  if(timer_entry_state == NULL)
    return;

  if(timer_entry_state->timer_entry_update_tag != 0) {
    g_source_remove(timer_entry_state->timer_entry_update_tag);
    timer_entry_state->timer_entry_update_tag = 0;
    _timer_button_swap(timer_entry_state);
    return;
  }

  if(timer_entry_state->timer_entry == NULL)
    return;

  timer_entry_state->timer_length = NULL;
  timer_entry_state->timer_entry_update_tag = g_timeout_add(1000, (GSourceFunc) _timer_entry_update, timer_entry_state);
  _timer_button_swap(timer_entry_state);
}


static gboolean _timer_entry_update(struct timer_entry_state* timer_entry_state) {
  if(timer_entry_state == NULL || timer_entry_state->timer_entry == NULL) {
    timer_entry_state->timer_entry_update_tag = 0;
    return FALSE;
  }

  GtkEntryBuffer* timer_entry_buffer = gtk_entry_get_buffer(timer_entry_state->timer_entry);

  int parsed_delay_buffer[3];
  parse_delay(gtk_entry_buffer_get_text(timer_entry_buffer), parsed_delay_buffer);

  if(timer_entry_state->timer_length == NULL) {
    timer_entry_state->timer_length = malloc(sizeof(int) * 3);
    memcpy(*timer_entry_state->timer_length, parsed_delay_buffer, sizeof(int) * 3);
  }

  const int empty_array[3] = {0, 0, 0};

  if(memcmp(parsed_delay_buffer, empty_array, sizeof(parsed_delay_buffer)) == 0) {
    timer_entry_state->timer_entry_update_tag = 0;
    _timer_button_swap(timer_entry_state);
    return FALSE;
  }

  parsed_delay_buffer[2]--;

  if(memcmp(parsed_delay_buffer, empty_array, sizeof(parsed_delay_buffer)) == 0) {
    timer_entry_state->timer_entry_update_tag = 0;
    _timer_notify(timer_entry_state);

    _timer_button_swap(timer_entry_state);
    _timer_entry_update_change_text(parsed_delay_buffer, timer_entry_buffer);

    return FALSE;
  }

  for(size_t i = 2; i > 0; i--) {
    if(parsed_delay_buffer[i] < 0) {
      parsed_delay_buffer[i] += 60;
      parsed_delay_buffer[i - 1] -= floor(parsed_delay_buffer[i] / 60) + 1;
    }
  }

  _timer_entry_update_change_text(parsed_delay_buffer, timer_entry_buffer);

  return TRUE;
}


static inline void _timer_entry_update_change_text(int parsed_delay[3], GtkEntryBuffer* timer_entry_buffer) {
  char* updated_delay_formated = format_delay(parsed_delay);
  gtk_entry_buffer_set_text(timer_entry_buffer, updated_delay_formated, strlen(updated_delay_formated));

  free(updated_delay_formated);
}


static void _timer_button_swap(struct timer_entry_state* timer_entry_state) {
  if(timer_entry_state->timer_button == NULL)
    return;

  char language_buffer[INI_LINE_DATA_SIZE];
  char translation_buffer[INI_LINE_DATA_SIZE];

  get_language(language_buffer);
  if(timer_entry_state->timer_entry_update_tag == 0)
    get_translation(translation_buffer, language_buffer, "start_timer", "Start Timer");
  else
    get_translation(translation_buffer, language_buffer, "stop_timer", "Stop Timer");

  gtk_button_set_label(timer_entry_state->timer_button, translation_buffer);
}


static void _timer_notify(struct timer_entry_state* timer_entry_state) {
  if(timer_entry_state == NULL || timer_entry_state->timer_length == NULL)
    return;

  char language_buffer[INI_LINE_DATA_SIZE];
  char translation_buffer[INI_LINE_DATA_SIZE];

  get_language(language_buffer);
  get_translation(translation_buffer, language_buffer, "ended_notification", "Timer %s ended");

  char* formated_delay = format_delay(*timer_entry_state->timer_length);

  size_t notification_title_length = strlen(translation_buffer) + strlen(formated_delay) + 1;
  char* notification_title = malloc(notification_title_length);
  snprintf(notification_title, notification_title_length, translation_buffer, formated_delay);
  free(formated_delay);

  GNotification* notification = g_notification_new(notification_title);
  free(notification_title);
  get_translation(translation_buffer, language_buffer, "accept_notification", "Okay");
  g_notification_add_button(notification, translation_buffer, "app.button");
  GApplication* app = g_application_get_default();
  g_application_send_notification(app, NULL, notification);

  g_object_unref(notification);
}
