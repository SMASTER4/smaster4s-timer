#pragma once

#include <gtk/gtk.h>

struct timer_entry_state {
  GtkEntry* timer_entry;
  guint timer_entry_update_tag;
};

extern int gui_run(int argc, char** argv);

static void _SIGNAL_app_activate(GtkApplication* app, gpointer user_data);

static void _SIGNAL_timer_entry_activate(GtkEntry* timer_entry, gpointer user_data);

static void _SIGNAL_timer_button_clicked(GtkButton* timer_button, gpointer user_data);

static void _timer_toggle(struct timer_entry_state* timer_entry_state);

static gboolean _timer_entry_update(struct timer_entry_state* timer_entry_state);
