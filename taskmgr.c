#include <gtk/gtk.h>
#include <glib.h>

#include <fcntl.h>
#include <unistd.h>

#include "taskmgr_sysinfo.c"

static void delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    g_print ("delete event occurred\n");
    gtk_main_quit ();
}

/* Another callback */
static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}

static void hide_window( GtkWidget *widget,
                         gpointer   data )
{
    gtk_widget_hide_all( widget );
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;

    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect (window, "delete-event",
              G_CALLBACK (delete_event), NULL);
    
    g_signal_connect (window, "destroy",
              G_CALLBACK (destroy), NULL);
    
    gtk_window_set_default_size (GTK_WINDOW (window), 960, 720);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    GtkWidget *notebook;
    notebook = gtk_notebook_new ();

    GtkWidget *tab_container_1;
    _SYSINFO sysinfo;
    tab_container_1 = gtk_vbox_new(FALSE, 10);
    sysinfo = sysinfo_init( tab_container_1 );

    gtk_notebook_append_page ((GtkNotebook *)notebook, tab_container_1, gtk_label_new ("System Info"));

    gtk_container_add (GTK_CONTAINER (window), notebook);

    gtk_widget_show_all (window);
    
    gtk_main ();
    
    return 0;
}