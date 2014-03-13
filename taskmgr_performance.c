#include <time.h>
#include <string.h>

struct _SYSPERF
{
    guint64 total;
    guint64 idle;
    gchar *cpu;
    guint64 mem_free;
    guint64 mem_all;
    GtkWidget *window;
};
typedef struct _SYSPERF _SYSPERF;

static void _performance_update( gpointer data )
{
    int fd;
    char buffer[256], recycle[32];
    ssize_t nread;

    _SYSPERF *_sysperf; 
    guint64 total_now, c_user, c_nice, c_system, c_idle, c_iowait, c_irq, c_softirq, c_usage;
    gchar *perf_text, _ts[64];
    time_t rawtime;

    _sysperf = (_SYSPERF *)data;

    fd = open("/proc/stat", O_RDONLY);
    if (fd < 0)
        return;
    nread = read(fd, buffer, sizeof buffer);
    sscanf(buffer, "%s %lu %lu %lu %lu %lu %lu %lu", recycle, &c_user, &c_nice, &c_system, &c_idle, &c_iowait, &c_irq, &c_softirq);
    total_now = c_user + c_nice + c_system + c_idle + c_iowait + c_irq + c_softirq;
    //g_print("%lu = %lu %lu %lu %lu %lu %lu %lu\n", total_now, c_user, c_nice, c_system, c_idle, c_iowait, c_irq, c_softirq);
    close(fd);
    if (_sysperf->total > 0UL && _sysperf->idle > 0UL)
    {
        c_usage = (total_now - _sysperf->total) - (c_idle - _sysperf->idle);
        //g_print("%lu / %lu\n", c_usage, total_now - _sysperf->total);
        g_free(_sysperf->cpu);
        _sysperf->cpu = g_strdup_printf("%.2f%%", 100.0 * (float) c_usage / (float) (total_now - _sysperf->total));
    }
    _sysperf->total = total_now;
    _sysperf->idle = c_idle;

    fd = open("/proc/meminfo", O_RDONLY);
    if (fd < 0)
        return;
    nread = read(fd, buffer, sizeof buffer);
    sscanf(buffer, "%s %lu %s\n%s %lu", recycle, &(_sysperf->mem_all), recycle, recycle, &(_sysperf->mem_free));
    close(fd);

    time ( &rawtime );
    strcpy(_ts, ctime ( &rawtime ));
    _ts[strlen(_ts) - 1] = '\0';
 
    perf_text = g_strdup_printf("%s - %s CPU - %lu Kb / %lu Kb", _ts, _sysperf->cpu, _sysperf->mem_free, _sysperf->mem_all);
    gtk_window_set_title ((GtkWindow *)_sysperf->window, perf_text);

    g_free(perf_text);
}

static _SYSPERF * performance_init( GtkWidget *window )
{
    _SYSPERF *_sysperf = malloc(sizeof (_SYSPERF));
    _sysperf->total = _sysperf->idle = _sysperf->mem_free = _sysperf->mem_all = 0UL;
    _sysperf->cpu = g_strdup( "0%" );
    _sysperf->window = window;

    return _sysperf;
}