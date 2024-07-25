#pragma once

#include <gtk/gtk.h>

extern int app_run(int argc, char** argv);

static void _SIGNAL_app_activate(GtkApplication* app, gpointer user_data);
