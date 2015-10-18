#include <gtk/gtk.h>

#include "gtk3shot.h"
#include "gtk3shotwin.h"

struct _Gtk3Shot
{
    GtkApplication parent;
};

struct _Gtk3ShotClass
{
    GtkApplicationClass parent_class;
};

G_DEFINE_TYPE(Gtk3Shot, gtk3shot, GTK_TYPE_APPLICATION);

static GdkPixbuf* gtk3shot_take_screenshot()
{
    GdkPixbuf* screenshot;
    GdkWindow* root_window;

    gint x, y;
    gint w, h;

    root_window = gdk_get_default_root_window();
    w = gdk_window_get_width(root_window);
    h = gdk_window_get_height(root_window);
    gdk_window_get_origin(root_window, &x, &y);

    screenshot = gdk_pixbuf_get_from_window(root_window, x, y, w, h);

    return screenshot;
}

static void gtk3shot_init(Gtk3Shot* app)
{
    
}

static void gtk3shot_activate(GApplication* app)
{
    Gtk3ShotWindow* win;
    GdkPixbuf* screenshot;

    win = gtk3shot_window_new(GTK3SHOT(app));
    screenshot = gtk3shot_take_screenshot();

    gtk3shot_window_open(win, screenshot);

    gtk_window_present(GTK_WINDOW(win));
}

static void gtk3shot_class_init(Gtk3ShotClass* class)
{
    G_APPLICATION_CLASS(class)->activate = gtk3shot_activate;
}

Gtk3Shot* gtk3shot_new(void)
{
    return g_object_new(GTK3SHOT_TYPE, "application-id", "tk.hackendoz.gtk3shot",
                        "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
