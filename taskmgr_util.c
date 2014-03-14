static void _exec (gchar * cmd)
{
	GError **err;
	g_spawn_command_line_async (cmd, err);
}

static void _exec_callback (GtkWidget *widget, gpointer data)
{
	GtkEntry *entry = (GtkEntry *)data;
	gchar *cmd = gtk_entry_get_text (entry);
	_exec (cmd);
}

static void _exec_shutdown (GtkWidget *widget, gpointer data)
{
	_exec ("shutdown");
}

static void util_init ( GtkWidget * container )
{
	GtkWidget *exec_input, *btn_exec_run, *btn_shutdown;
	GtkEntryBuffer * exec_buffer;
	exec_buffer = gtk_entry_buffer_new ("", 0);
	exec_input = gtk_entry_new_with_buffer ((GtkEntryBuffer *) exec_buffer);
	btn_exec_run = gtk_button_new_with_label ("Run");
	btn_shutdown = gtk_button_new_with_label ("Shutdown");

	gtk_container_add (GTK_CONTAINER (container), exec_input);
	gtk_container_add (GTK_CONTAINER (container), btn_exec_run);
	gtk_container_add (GTK_CONTAINER (container), btn_shutdown);

	g_signal_connect ( btn_exec_run, "clicked", G_CALLBACK (_exec_callback), exec_input );
	g_signal_connect ( btn_shutdown, "clicked", G_CALLBACK (_exec_shutdown), NULL );
}