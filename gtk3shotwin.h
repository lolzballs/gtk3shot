#ifndef __GTK3SHOTWIN_H
#define __GTK3SHOTWIN_H

#include <gtk/gtk.h>

#include "gtk3shot.h"

#define GTK3SHOT_WINDOW_TYPE (gtk3shot_window_get_type())
#define GTK3SHOT_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK3SHOT_WINDOW_TYPE), Gtk3Window)

typedef struct _Gtk3ShotWindow Gtk3ShotWindow;
typedef struct _Gtk3ShotWindowClass Gtk3ShotWindowClass;

GType gtk3shot_window_get_type(void);
Gtk3ShotWindow* gtk3shot_window_new(Gtk3Shot* app);
void gtk3shot_window_open(Gtk3ShotWindow* win, GdkPixbuf* screenshot);

#endif
