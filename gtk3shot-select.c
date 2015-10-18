#include <gtk/gtk.h>

#include "gtk3shot-select.h"

typedef struct {
    GdkRectangle rect;
    gboolean button_pressed;
    GtkWidget* window;

    gboolean aborted;
} select_area_data;

static gboolean select_area_key_press(GtkWidget* window, GdkEventKey* event, select_area_data* data)
{
    if (event->keyval == GDK_KEY_Escape)
    {
        data->rect.x = 0;
        data->rect.y = 0;
        data->rect.width = 0;
        data->rect.height = 0;

        g_print("Escape\n");
        gtk_main_quit();
    }

    return TRUE;
}

static gboolean select_area_button_press(GtkWidget* window, GdkEventButton* event, select_area_data* data)
{
    if (data->button_pressed)
    {
        return TRUE;
    }

    data->button_pressed = TRUE;
    data->rect.x = event->x_root;
    data->rect.y = event->y_root;

    return TRUE;
}

static gboolean select_area_button_release(GtkWidget* window, GdkEventButton* event, select_area_data* data)
{
    if (!data->button_pressed)
    {
        return TRUE;
    }

    data->rect.width = ABS(data->rect.x - event->x_root);
    data->rect.height = ABS(data->rect.y - event->y_root);
    data->rect.x = MIN(data->rect.x, event->x_root);
    data->rect.y = MIN(data->rect.y, event->y_root);

    if (data->rect.width == 0 || data->rect.height == 0)
    {
        data->aborted = TRUE;
    }

    gtk_main_quit();

    return TRUE;
}

static gboolean select_area_motion_notify(GtkWidget* window, GdkEventMotion* event, select_area_data* data)
{
    GdkRectangle draw_rect;

    if (!data->button_pressed)
    {
        return TRUE;
    }

    draw_rect.width = ABS(data->rect.x - event->x_root);
    draw_rect.height = ABS(data->rect.y - event->y_root);
    draw_rect.x = MIN(data->rect.x, event->x_root);
    draw_rect.y = MIN(data->rect.y, event->y_root);

    if (draw_rect.width <= 0 || draw_rect.height <= 0)
    {
        gtk_window_move(GTK_WINDOW(window), -100, -100);
        gtk_window_resize(GTK_WINDOW(window), 10, 10);
        return TRUE;
    }

    gtk_window_move(GTK_WINDOW(window), draw_rect.x, draw_rect.y);
    gtk_window_resize(GTK_WINDOW(window), draw_rect.width, draw_rect.height);

    if (!gtk_widget_get_app_paintable(window))
    {
        GdkWindow* gdkwindow = gtk_widget_get_window(window);
        if (draw_rect.width > 2 && draw_rect.height > 2)
        {
            cairo_region_t* region;
            cairo_rectangle_int_t region_rect = {
                0, 0,
                draw_rect.width, draw_rect.height
            };

            region = cairo_region_create_rectangle(&region_rect);
            region_rect.x++;
            region_rect.y++;
            region_rect.width -= 2;
            region_rect.height -= 2;
            cairo_region_subtract_rectangle(region, &region_rect);

            gdk_window_shape_combine_region(gdkwindow, region, 0, 0);

            cairo_region_destroy(region);
        }
        else
        {
            gdk_window_shape_combine_region(gdkwindow, NULL, 0, 0);
        }
    }

    return TRUE;
}

static gboolean select_window_draw(GtkWidget* window, cairo_t* cr)
{
    GtkStyleContext* style;

    style = gtk_widget_get_style_context(window);

    if (gtk_widget_get_app_paintable(window))
    {
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        cairo_set_source_rgba(cr, 0, 0, 0, 0);
        cairo_paint(cr);

        gtk_style_context_save(style);
        gtk_style_context_add_class(style, GTK_STYLE_CLASS_RUBBERBAND);

        gtk_render_background(style, cr, 0, 0,
                              gtk_widget_get_allocated_width(window),
                              gtk_widget_get_allocated_height(window));
        gtk_style_context_restore(style);
    }

    return TRUE;
}

static GtkWidget* create_select_window(void)
{
    GtkWidget* window;
    GdkScreen* screen;
    GdkVisual* visual;

    screen = gdk_screen_get_default();
    visual = gdk_screen_get_rgba_visual(screen);

    window = gtk_window_new(GTK_WINDOW_POPUP);
    if (gdk_screen_is_composited(screen) && visual)
    {
        gtk_widget_set_visual(window, visual);
        gtk_widget_set_app_paintable(window, TRUE);
    }

    g_signal_connect(window, "draw", G_CALLBACK(select_window_draw), NULL);

    gtk_window_move(GTK_WINDOW(window), -100, -100);
    gtk_window_resize(GTK_WINDOW(window), 10, 10);
    gtk_widget_show(window);

    return window;
}

GdkRectangle gtk3shot_select_area(void)
{
    select_area_data data;
    GdkDeviceManager* manager;
    GdkDevice* pointer;
    GdkDevice* keyboard;
    GdkGrabStatus res;

    data.rect.x = 0;
    data.rect.y = 0;
    data.rect.width = 0;
    data.rect.height = 0;
    data.button_pressed = FALSE;
    data.aborted = FALSE;
    data.window = create_select_window();

    g_signal_connect(data.window, "key-press-event", G_CALLBACK(select_area_key_press), &data);
    g_signal_connect(data.window, "button-press-event", G_CALLBACK(select_area_button_press), &data);
    g_signal_connect(data.window, "button-release-event", G_CALLBACK(select_area_button_release), &data);
    g_signal_connect(data.window, "motion-notify-event", G_CALLBACK(select_area_motion_notify), &data);

    manager = gdk_display_get_device_manager(gdk_display_get_default());
    pointer = gdk_device_manager_get_client_pointer(manager);
    keyboard = gdk_device_get_associated_device(pointer);

    res = gdk_device_grab(pointer, gtk_widget_get_window(data.window),
                          GDK_OWNERSHIP_NONE, FALSE,
                          GDK_POINTER_MOTION_MASK |
                          GDK_BUTTON_PRESS_MASK |
                          GDK_BUTTON_RELEASE_MASK,
                          NULL, GDK_CURRENT_TIME);

    if (res != GDK_GRAB_SUCCESS)
    {
        g_error("Couldn't grab pointer");
    }

    res = gdk_device_grab(keyboard, gtk_widget_get_window(data.window),
                          GDK_OWNERSHIP_NONE, FALSE,
                          GDK_KEY_PRESS_MASK |
                          GDK_KEY_RELEASE_MASK,
                          NULL, GDK_CURRENT_TIME);

    if (res != GDK_GRAB_SUCCESS)
    {
        gdk_device_ungrab(pointer, GDK_CURRENT_TIME);
        g_error("Couldn't grab keyboard");
    }

    gtk_main();

    gdk_device_ungrab(pointer, GDK_CURRENT_TIME);
    gdk_device_ungrab(keyboard, GDK_CURRENT_TIME);

    gtk_widget_destroy(data.window);
    gdk_flush();

    return data.rect;
}
