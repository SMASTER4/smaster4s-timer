#pragma once

#include <gtk/gtk.h>

struct application_state {
  GtkEntry* timer_entry;
  GtkButton* timer_button;
  guint timer_entry_update_tag;
  int (*timer_length)[3];
};

extern int gui_run(int argc, char** argv);

static void _SIGNAL_app_activate(GtkApplication* app, gpointer user_data);

static void _SIGNAL_timer_entry_activate(GtkEntry* timer_entry, gpointer user_data);

static void _SIGNAL_timer_button_clicked(GtkButton* timer_button, gpointer user_data);

static void _load_css();

static void _timer_toggle(struct application_state* application_state);

static gboolean _timer_entry_update(struct application_state* application_state);

static inline void _timer_entry_update_change_text(int parsed_delay[3], GtkEntry* timer_entry);

static void _timer_button_swap(struct application_state* application_state);

static void _timer_notify(struct application_state* application_state);
