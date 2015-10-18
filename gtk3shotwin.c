#include <gtk/gtk.h>

#include "gtk3shot.h"
#include "gtk3shotwin.h"

struct _Gtk3ShotWindow
{
    GtkApplicationWindow parent;
};

struct _Gtk3ShotWindowClass
{
    GtkApplicationWindowClass parent_class;
};

G_DEFINE_TYPE(Gtk3ShotWindow, gtk3shot_window, GTK_TYPE_APPLICATION_WINDOW);

static void gtk3shot_window_init(Gtk3ShotWindow* app)
{
    
}

static void gtk3shot_window_class_init(Gtk3ShotWindowClass* class)
{
    
}

Gtk3ShotWindow* gtk3shot_window_new(Gtk3Shot* app)
{
    return g_object_new(GTK3SHOT_WINDOW_TYPE, "application", app, NULL);
}

void gtk3shot_window_open(Gtk3ShotWindow* win, GdkPixbuf* screenshot)
{
    GtkWidget* image;

    image = gtk_image_new_from_pixbuf(screenshot);

    gtk_widget_show(image);
    gtk_container_add(GTK_CONTAINER(win), image);
}
