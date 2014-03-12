#include <gtk/gtk.h>
#include <glib.h>

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

static gboolean out_watch( GIOChannel *channel,
                           GIOCondition cond,
                           GtkTextBuffer *out_buf )
{
    gchar *string;
    gsize  size;

    if( cond == G_IO_HUP )
    {
        g_io_channel_unref( channel );
        return( FALSE );
    }

    g_io_channel_read_line( channel, &string, &size, NULL, NULL );
    gtk_text_buffer_insert_at_cursor( out_buf, string, -1 );
    g_free( string );

    return( TRUE );
}

static gboolean proc_create( GtkWidget *window, 
                             gchar **argv )
{
    GPid proc_pid;
    gint proc_stdout;

    GtkTextBuffer *out_buf = gtk_text_buffer_new(NULL);
    GtkWidget *output = gtk_text_view_new_with_buffer(out_buf);
    gtk_container_add (GTK_CONTAINER (window), output);
    gtk_widget_show_all(window);

    gboolean proc_status = g_spawn_async_with_pipes ( "./",
                                                      argv, /* gchar **argv, */
                                                      NULL, /* gchar **envp, */
                                                      0, /* GSpawnFlags flags, */
                                                      NULL, /* GSpawnChil   dSetupFunc child_setup, */
                                                      NULL, /* gpointer user_data, */
                                                      &proc_pid, /* GPid *child_pid, */
                                                      NULL, /* gint *standard_input, */
                                                      &proc_stdout, /* gint *standard_output, */
                                                      NULL, /* gint *standard_error, */
                                                      NULL); /* GError **error */
    g_print("pid: %d\n", proc_pid);
    GIOChannel *out_ch;
    out_ch = g_io_channel_unix_new( proc_stdout );
    g_io_add_watch( out_ch, G_IO_IN | G_IO_HUP, (GIOFunc)out_watch, out_buf );
    //g_print("Process has exited.\n");
    return proc_status;
}

static void start_proc_1( GtkWidget *widget,
                          gpointer   data )
{
    GtkWidget *window = (GtkWidget *)data;
    gchar *argv[] = {"./bin/count.o", "5", NULL};
    proc_create(window, argv);
}

static void start_proc_2( GtkWidget *widget,
                          gpointer   data )
{
    GtkWidget *window = (GtkWidget *)data;
    gchar *argv[] = {"./bin/count.o", "10", NULL};
    proc_create(window, argv);
}

static void start_proc_3( GtkWidget *widget,
                          gpointer   data )
{
    GtkWidget *window = (GtkWidget *)data;
    gchar *argv[] = {"./bin/count.o", "20", NULL};
    proc_create(window, argv);
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
    pWin_1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    pWin_2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    pWin_3 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW (pWin_1), 480, 360);
    gtk_window_set_default_size (GTK_WINDOW (pWin_2), 480, 360);
    gtk_window_set_default_size (GTK_WINDOW (pWin_3), 480, 360);

    g_signal_connect (pWin_1, "delete-event", G_CALLBACK (hide_window), NULL);
    g_signal_connect (pWin_2, "delete-event", G_CALLBACK (hide_window), NULL);
    g_signal_connect (pWin_3, "delete-event", G_CALLBACK (hide_window), NULL);
    g_signal_connect (btn_1, "clicked", G_CALLBACK (start_proc_1), pWin_1);
    g_signal_connect (btn_2, "clicked", G_CALLBACK (start_proc_2), pWin_2);
    g_signal_connect (btn_3, "clicked", G_CALLBACK (start_proc_3), pWin_3);
    

    gtk_widget_show_all (window);
    
    gtk_main ();
    
    return 0;
}