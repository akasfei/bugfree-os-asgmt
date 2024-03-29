#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include "taskmgr_sysinfo.c"
#include "taskmgr_process.c"
#include "taskmgr_performance.c"
#include "taskmgr_util.c"

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
/*
static void hide_window( GtkWidget *widget,
                         gpointer   data )
{
    gtk_widget_hide_all( widget );
}
*/
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
    
    gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

    /* Notebook */

    GtkWidget *notebook;
    notebook = gtk_notebook_new ();

    /* Notebook tab 1, system info */

    GtkWidget *tab_container_1;
    _SYSINFO sysinfo_widget;
    tab_container_1 = gtk_vbox_new(FALSE, 10);
    sysinfo_widget = sysinfo_init( tab_container_1 );

    /* Notebook tab 2, process */

    GtkWidget *tab_container_2, *scroll_window, *btn_group, *btn_refresh_proc, *btn_kill_proc;
    _SYSPROC *sysproc_widget;
    tab_container_2 = gtk_vbox_new (FALSE, 10);
    btn_group = gtk_hbox_new (FALSE, 10);
    btn_refresh_proc = gtk_button_new_with_label( "Refresh" );
    btn_kill_proc = gtk_button_new_with_label( "Kill Process" );
    scroll_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_widget_set_size_request (scroll_window, -1, 420);
    sysproc_widget = process_init();
    gtk_container_add (GTK_CONTAINER (scroll_window), sysproc_widget->view);
    gtk_container_add (GTK_CONTAINER (btn_group), btn_refresh_proc);
    gtk_container_add (GTK_CONTAINER (btn_group), btn_kill_proc);
    gtk_container_add (GTK_CONTAINER (tab_container_2), scroll_window);
    gtk_container_add (GTK_CONTAINER (tab_container_2), btn_group);
    g_signal_connect ( btn_refresh_proc, "clicked", G_CALLBACK (_process_refresh), sysproc_widget );
    g_signal_connect ( btn_kill_proc, "clicked", G_CALLBACK (_process_kill), sysproc_widget );

    /* Display system Performance */

    GtkWidget *tab_container_3;
    _SYSPERF *_sysperf;
    tab_container_3 = gtk_vbox_new (FALSE, 10);
    _sysperf = performance_init( window );
    gtk_container_add (GTK_CONTAINER (tab_container_3), _sysperf->textarea);
    gtk_container_add (GTK_CONTAINER (tab_container_3), _sysperf->btn_refresh);
    gtk_container_add (GTK_CONTAINER (tab_container_3), _sysperf->btn_auto);

    g_timeout_add_seconds (1, (GSourceFunc)_performance_update, _sysperf);

    /* Util */
    GtkWidget *tab_container_4;
    tab_container_4 = gtk_vbox_new ( FALSE, 10 );
    util_init( tab_container_4 );

    /* Adding tabs to notebook */

    gtk_notebook_append_page ((GtkNotebook *)notebook, tab_container_1, gtk_label_new ("System Info"));
    gtk_notebook_append_page ((GtkNotebook *)notebook, tab_container_2, gtk_label_new ("Process"));
    gtk_notebook_append_page ((GtkNotebook *)notebook, tab_container_3, gtk_label_new ("Performance"));
    gtk_notebook_append_page ((GtkNotebook *)notebook, tab_container_4, gtk_label_new ("Utilities"));

    gtk_container_add (GTK_CONTAINER (window), notebook);

    gtk_widget_show_all (window);
    
    gtk_main ();

    //free(sysproc_widget);
    //free(_sysperf);
    
    return 0;
}