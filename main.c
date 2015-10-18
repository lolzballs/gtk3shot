#include <gtk/gtk.h>

#include "gtk3shot.h"

int main(int argc, char** argv)
{
    return g_application_run(G_APPLICATION(gtk3shot_new()), argc, argv);
}
