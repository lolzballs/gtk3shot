#ifndef __GTK3SHOT_H
#define __GTK3SHOT_H

#include <gtk/gtk.h>

#define GTK3SHOT_TYPE (gtk3shot_get_type())
#define GTK3SHOT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK3SHOT_TYPE, Gtk3Shot))

typedef struct _Gtk3Shot Gtk3Shot;
typedef struct _Gtk3ShotClass Gtk3ShotClass;

GType gtk3shot_get_type(void);
Gtk3Shot* gtk3shot_new(void);

#endif
