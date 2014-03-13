#include <dirent.h>

enum
{
   PID_COL,
   NAME_COL,
   STATE_COL,
   CPU_COL,
   MEM_COL,
   COL_COUNT
};

struct _SYSPROC
{
    GtkWidget *view;
    GtkTreeStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *columns[COL_COUNT];
};
typedef struct _SYSPROC _SYSPROC;

struct _PROC_INFO
{
    gint pid;
    gchar *name;
    gchar *state;
    gchar *cpu;
    gchar *mem;
};
typedef struct _PROC_INFO _PROC_INFO;

static void free_proc_info( _PROC_INFO _proc_info )
{
    g_free( _proc_info.name );
    g_free( _proc_info.state );
    g_free( _proc_info.cpu );
    g_free( _proc_info.mem );
}

static _PROC_INFO process_stat( gint pid )
{
    int fd;
    gchar buffer[4096], *proc_path, b_name[128], b_state;
    gint b_mem;
    ssize_t nread;

    _PROC_INFO _proc_info;

    proc_path = g_strdup_printf("/proc/%d/stat", pid);
    fd = open(proc_path, O_RDONLY);
    if (fd < 0)
        return _proc_info;
    nread = read(fd, buffer, sizeof buffer);
    sscanf(buffer, "%d %s %c", &(_proc_info.pid), b_name, &b_state);
    close(fd);

    proc_path = g_strdup_printf("/proc/%d/statm", pid);
    fd = open(proc_path, O_RDONLY);
    if (fd < 0)
        return _proc_info;
    nread = read(fd, buffer, sizeof buffer);
    sscanf(buffer, "%d", &b_mem);
    close(fd);

    g_free(proc_path);

    _proc_info.name = g_strdup( b_name );
    _proc_info.state = g_strdup_printf("%c", b_state);
    _proc_info.cpu = g_strdup_printf("00");
    _proc_info.mem = g_strdup_printf("%d KB", b_mem);

    return _proc_info;
}

static void process_update( _SYSPROC *_sysproc )
{
    GtkTreeIter iter;
    DIR *proc_dir;
    struct dirent *proc_dirent;
    _PROC_INFO this_proc;

    proc_dir = opendir("/proc");
    if (proc_dir == NULL)
    {
        g_print("Cannot read directory /proc \n");
        return;
    }

    gtk_tree_store_clear (_sysproc->store);
    while((proc_dirent = readdir(proc_dir)) != NULL)
    {
        if (proc_dirent->d_name[0] < '0' || proc_dirent->d_name[0] > '9')
            continue;
        gint pid = atoi(proc_dirent->d_name);
        this_proc = process_stat( pid );

        gtk_tree_store_append (_sysproc->store, &iter, NULL);  /* Acquire a top-level iterator */
        gtk_tree_store_set (_sysproc->store, &iter,
                            PID_COL, this_proc.pid,
                            NAME_COL, this_proc.name,
                            STATE_COL, this_proc.state,
                            CPU_COL, this_proc.cpu,
                            MEM_COL, this_proc.mem,
                            -1);

        free_proc_info( this_proc );
    }
    closedir(proc_dir);
    
}

static _SYSPROC process_init()
{
    _SYSPROC _sysproc;

    _sysproc.store = gtk_tree_store_new (COL_COUNT,       /* Total number of columns */
                                         G_TYPE_INT,      /* PID                     */
                                         G_TYPE_STRING,   /* Process Name            */
                                         G_TYPE_STRING,   /* Process state           */
                                         G_TYPE_STRING,   /* CPU usage               */
                                         G_TYPE_STRING);  /* MEM usage               */
    _sysproc.view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (_sysproc.store));
    _sysproc.renderer = gtk_cell_renderer_text_new ();
    _sysproc.columns[PID_COL] = gtk_tree_view_column_new_with_attributes ("PID",
                                                                          _sysproc.renderer,
                                                                          "text", PID_COL,
                                                                          NULL);
    _sysproc.columns[NAME_COL] = gtk_tree_view_column_new_with_attributes ("Name",
                                                                           _sysproc.renderer,
                                                                           "text", NAME_COL,
                                                                           NULL);
    _sysproc.columns[STATE_COL] = gtk_tree_view_column_new_with_attributes ("State",
                                                                            _sysproc.renderer,
                                                                            "text", STATE_COL,
                                                                            NULL);
    _sysproc.columns[CPU_COL] = gtk_tree_view_column_new_with_attributes ("CPU",
                                                                          _sysproc.renderer,
                                                                          "text", CPU_COL,
                                                                          NULL);
    _sysproc.columns[MEM_COL] = gtk_tree_view_column_new_with_attributes ("Memory",
                                                                          _sysproc.renderer,
                                                                          "text", MEM_COL,
                                                                          NULL);
    int i;
    for (i = 0; i < COL_COUNT; i++)
        gtk_tree_view_append_column (GTK_TREE_VIEW (_sysproc.view), _sysproc.columns[i]);

    process_update( &_sysproc );
    return _sysproc;
}

static void _process_refresh( GtkWidget *widget, gpointer data )
{
    process_update( (_SYSPROC *)data );
}

static void _process_kill( GtkWidget *widget, gpointer data )
{
    gint pid;
    GtkTreeIter iter;
    GtkTreeModel *model;
    GtkTreeSelection *select;
    _SYSPROC *_sysproc = (_SYSPROC *)data;

    select = gtk_tree_view_get_selection (GTK_TREE_VIEW (_sysproc->view));
    gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
    if (gtk_tree_selection_get_selected (select, &model, &iter))
    {
            gtk_tree_model_get (model, &iter, PID_COL, &pid, -1);
            g_print ("Killing process %d\n", pid);
            kill(pid, 1);
            process_update( _sysproc );
    }
}