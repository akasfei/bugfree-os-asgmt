#include <gtk/gtk.h>
#include <glib.h>

/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below. */
static void hello( GtkWidget *widget,
                   gpointer   data )
{
    g_print ("Hello World\n");
}

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

static gboolean proc_create( GtkWidget *window, 
                             GtkWidget *output,
                             gchar **argv )
{
    GPid proc_pid;
    gint proc_stdout;
    gboolean proc_status = g_spawn_async_with_pipes ( "./",
                                                      argv, /* gchar **argv, */
                                                      NULL, /* gchar **envp, */
                                                      0777, /* GSpawnFlags flags, */
                                                      NULL, /* GSpawnChil   dSetupFunc child_setup, */
                                                      NULL, /* gpointer user_data, */
                                                      &proc_pid, /* GPid *child_pid, */
                                                      NULL, /* gint *standard_input, */
                                                      &proc_stdout, /* gint *standard_output, */
                                                      NULL, /* gint *standard_error, */
                                                      NULL); /* GError **error */
    GtkTextBuffer *out_buf = gtk_text_buffer_new(NULL);
    gtk_text_view_set_buffer((GtkTextView *)output, out_buf);
    gtk_widget_show_all(window);
    char buffer[4096];
    while (!kill(proc_pid, 0))
    {
        ssize_t count = read(proc_stdout, buffer, sizeof (buffer));
        if (count == -1) {
            break;
        } else if (count == 0) {
            continue;
        } else {
            gtk_text_buffer_insert_at_cursor( out_buf, buffer, count );
            gtk_text_view_set_buffer( output, out_buf );
        }
    }
    return proc_status;
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
    
    gtk_window_set_default_size (GTK_WINDOW (window), 320, 240);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    /* Button container */
    GtkWidget *vbox;
    vbox = gtk_vbox_new(TRUE, 4);

    gtk_container_add (GTK_CONTAINER (window), vbox);
    
    /* Buttons */
    GtkWidget *btn_1, *btn_2, *btn_3;
    btn_1 = gtk_button_new_with_label ("Process 1");
    btn_2 = gtk_button_new_with_label ("Process 2");
    btn_3 = gtk_button_new_with_label ("Process 3");

    gtk_container_add (GTK_CONTAINER (vbox), btn_1);
    gtk_container_add (GTK_CONTAINER (vbox), btn_2);
    gtk_container_add (GTK_CONTAINER (vbox), btn_3);

    /* Windows for showing each process */
    GtkWidget *pWin_1, *pWin_2, *pWin_3;
    gtk_window_set_default_size (GTK_WINDOW (pWin_1), 480, 360);
    gtk_window_set_default_size (GTK_WINDOW (pWin_2), 480, 360);
    gtk_window_set_default_size (GTK_WINDOW (pWin_3), 480, 360);

    /* Text fields for each window */
    GtkWidget *pout_1, *pout_2, *pout_3;
    pout_1 = gtk_text_view_new();
    pout_2 = gtk_text_view_new();
    pout_3 = gtk_text_view_new();

    gtk_container_add (GTK_CONTAINER (pWin_1), pout_1);
    gtk_container_add (GTK_CONTAINER (pWin_2), pout_2);
    gtk_container_add (GTK_CONTAINER (pWin_3), pout_3);
    
    g_signal_connect (btn_1, "clicked",
              G_CALLBACK (hello), NULL);

    gtk_widget_show_all (window);
    
    gtk_main ();
    
    return 0;
}