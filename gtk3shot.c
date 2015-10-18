#include <gtk/gtk.h>

#include "gtk3shot.h"
#include "gtk3shotwin.h"
#include "gtk3shot-select.h"

struct _Gtk3Shot
{
    GtkApplication parent;
};

struct _Gtk3ShotClass
{
    GtkApplicationClass parent_class;
};

G_DEFINE_TYPE(Gtk3Shot, gtk3shot, GTK_TYPE_APPLICATION);

static GdkPixbuf* gtk3shot_take_screenshot(GdkRectangle rectangle)
{
    GdkPixbuf* screenshot;
    GdkWindow* root_window;

    root_window = gdk_get_default_root_window();
    screenshot = gdk_pixbuf_get_from_window(root_window, rectangle.x, rectangle.y, rectangle.width, rectangle.height);

    return screenshot;
}

static void gtk3shot_open_preview(GApplication* app, GdkPixbuf* screenshot)
{
    Gtk3ShotWindow* win;

    win = gtk3shot_window_new(GTK3SHOT(app));
    gtk3shot_window_open(win, screenshot);
    gtk_window_present(GTK_WINDOW(win));
}

static void gtk3shot_init(Gtk3Shot* app)
{
}

static void gtk3shot_activate(GApplication* app)
{
    GdkPixbuf* screenshot;
    GdkRectangle select_area;

    select_area = gtk3shot_select_area();
    screenshot = gtk3shot_take_screenshot(select_area);

    g_debug("w: %d, h: %d, x: %d, y: %d\n", select_area.width, select_area.height, select_area.x, select_area.y);

    gtk3shot_open_preview(app, screenshot);
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
