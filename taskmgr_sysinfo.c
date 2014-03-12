struct sysinfo 
{
    GtkWidget *hostname;
    GtkWidget *uptime;
    GtkWidget *upduration;
    GtkWidget *ostype;
    GtkWidget *osrelease;
    GtkWidget *cpu;
};
typedef struct sysinfo _SYSINFO;

static void sysinfo_update( _SYSINFO *_sysinfo )
{
    int fd;
    char buffer[4096];
    ssize_t nread;
    
    fd = open("/proc/sys/kernel/hostname", O_RDONLY);
    if (fd < 0)
        return;
    nread = read(fd, buffer, sizeof buffer);
    gtk_label_set_text( (GtkLabel *) _sysinfo->hostname, g_strndup ( buffer, nread ) );
    close(fd);

    gchar **uptime_text, *gbuffer;
    fd = open("/proc/uptime", O_RDONLY);
    if (fd < 0)
        return;
    nread = read(fd, buffer, sizeof buffer);
    gbuffer = g_strndup ( buffer, nread );
    uptime_text = g_regex_split_simple ( " ", gbuffer, 0, 0 );
    gtk_label_set_text( (GtkLabel *) _sysinfo->uptime, g_strdup ( uptime_text[0] ) );
    gtk_label_set_text( (GtkLabel *) _sysinfo->upduration, g_strdup ( uptime_text[1] ) );
    g_strfreev( uptime_text );
    g_free( gbuffer );
    close(fd);

    fd = open("/proc/sys/kernel/ostype", O_RDONLY);
    if (fd < 0)
        return;
    nread = read(fd, buffer, sizeof buffer);
    gtk_label_set_text( (GtkLabel *) _sysinfo->ostype, g_strndup ( buffer, nread ) );
    close(fd);

    fd = open("/proc/sys/kernel/osrelease", O_RDONLY);
    if (fd < 0)
        return;
    nread = read(fd, buffer, sizeof buffer);
    gtk_label_set_text( (GtkLabel *) _sysinfo->osrelease, g_strndup ( buffer, nread ) );
    close(fd);

    fd = open("/proc/cpuinfo", O_RDONLY);
    if (fd < 0)
        return;
    nread = read(fd, buffer, sizeof buffer);
    gbuffer = g_strndup ( buffer, nread );
    GRegex *regex;
    GMatchInfo *match_info;
    regex = g_regex_new ("(?<=(model name)).+", 0, 0, NULL);
    g_regex_match (regex, gbuffer, 0, &match_info);
    if (g_match_info_matches (match_info))
    {
        gchar *word = g_match_info_fetch (match_info, 0);
        gtk_label_set_text( (GtkLabel *) _sysinfo->cpu, word + 2 );
    } else
        g_print("CPUID - Regexp error.\n");
    g_free( gbuffer );
    g_match_info_free (match_info);
    g_regex_unref (regex);
    close(fd);
}

static _SYSINFO sysinfo_init( GtkWidget *container )
{
    _SYSINFO _sysinfo;

    GtkWidget *hbox, *label_container_left, *label_container_right;
    hbox = gtk_hbox_new(TRUE, 10);
    gtk_container_add (GTK_CONTAINER (container), hbox);
    label_container_left = gtk_vbox_new(FALSE, 10);
    label_container_right = gtk_vbox_new(FALSE, 10);
    gtk_container_add (GTK_CONTAINER (hbox), label_container_left);
    gtk_container_add (GTK_CONTAINER (hbox), label_container_right);

    gtk_container_add (GTK_CONTAINER (label_container_left), gtk_label_new ("Hostname: "));
    gtk_container_add (GTK_CONTAINER (label_container_left), gtk_label_new ("System up time: "));
    gtk_container_add (GTK_CONTAINER (label_container_left), gtk_label_new ("System up duration: "));
    gtk_container_add (GTK_CONTAINER (label_container_left), gtk_label_new ("Operating System: "));
    gtk_container_add (GTK_CONTAINER (label_container_left), gtk_label_new ("Operating System release: "));
    gtk_container_add (GTK_CONTAINER (label_container_left), gtk_label_new ("CPU Information :"));

    _sysinfo.hostname = gtk_label_new ("HOSTNAME UNKNOWN");
    _sysinfo.uptime = gtk_label_new ("SYSTEM UPTIME UNKNOWN");
    _sysinfo.upduration = gtk_label_new ("SYSTEM UPDURATION UNKNOWN");
    _sysinfo.ostype = gtk_label_new ("OS TYPE UNKNOWN");
    _sysinfo.osrelease = gtk_label_new ("OS RELEASE UNKNOWN");
    _sysinfo.cpu = gtk_label_new ("CPU TYPE UNKNOWN");

    gtk_container_add (GTK_CONTAINER (label_container_right), _sysinfo.hostname);
    gtk_container_add (GTK_CONTAINER (label_container_right), _sysinfo.uptime);
    gtk_container_add (GTK_CONTAINER (label_container_right), _sysinfo.upduration);
    gtk_container_add (GTK_CONTAINER (label_container_right), _sysinfo.ostype);
    gtk_container_add (GTK_CONTAINER (label_container_right), _sysinfo.osrelease);
    gtk_container_add (GTK_CONTAINER (label_container_right), _sysinfo.cpu);

    sysinfo_update( &_sysinfo );
    return _sysinfo;
}
