#pragma once

#include <gtk/gtk.h>

extern int gui_run(int argc, char** argv);

static void _SIGNAL_app_activate(GtkApplication* app, gpointer user_data);

static void _SIGNAL_timer_entry_activate(GtkEntry* timer_entry, gpointer user_data);

static void _SIGNAL_timer_button_clicked(GtkButton* timer_button, gpointer user_data);

static gboolean _timer_entry_update(GtkEntry* timer_entry);
