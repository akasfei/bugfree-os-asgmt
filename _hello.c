#include <gtk/gtk.h>

/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below. */
static void hello( GtkWidget *widget,
                   gpointer   data )
{
    g_print ("Hello World\n");
}

static gboolean delete_event( GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data )
{
    g_print ("delete event occurred\n");
    return TRUE;
}

/* Another callback */
static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;

    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    g_signal_connect (window, "delete-event",
              G_CALLBACK (delete_event), NULL);
    
    g_signal_connect (window, "destroy",
              G_CALLBACK (destroy), NULL);
    
    gtk_window_set_default_size (GTK_WINDOW (window), 320, 240);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    /* Creates a new button with the label "Hello World". */
    button = gtk_button_new_with_label ("Hello World");
    
    g_signal_connect (button, "clicked",
              G_CALLBACK (hello), NULL);

    g_signal_connect_swapped (button, "clicked",
                  G_CALLBACK (gtk_widget_destroy),
                              window);

    gtk_container_add (GTK_CONTAINER (window), button);

    gtk_widget_show (button);

    gtk_widget_show (window);
    
    gtk_main ();
    
    return 0;
}