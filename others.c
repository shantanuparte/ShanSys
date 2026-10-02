#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

// Battry

int get_battery_percentage()
{
    const char *names[] = {"BATO", "BAT1", "BATT", "CMB0"};
    int cap = -1;
    char path[128];

    for (int i = 0; i < 4; i++)
    {
        snprintf(path, sizeof(path), "/sys/class/power_supply/%s/capacity", names[i]);

        FILE *battry_ptr = fopen(path, "r");
        if (battry_ptr != NULL)
        {
            if (fscanf(battry_ptr, "%d", &cap) != 1)
            {
                cap = -1;
            }
            fclose(battry_ptr);
            if (cap >= 0)
            {
                break;
            }
        }
    }

    return cap;
}

// avg load

double get_load_avg()
{
    double load = 0.0;
    FILE *fp = fopen("/proc/loadavg", "r");
    if (fp != NULL)
    {
        fscanf(fp, "%lf", &load);
        fclose(fp);
    }

    return load;
}

// up time

void get_uptime(char *buff, size_t size)
{
    FILE *up_ptr = fopen("/proc/uptime", "r");
    if (up_ptr == NULL)
    {
        return;
    }

    double sec;
    if (fscanf(up_ptr, "%lf", &sec) == 1)
    {
        int hours = (int)(sec / 3600);
        int mins = (int)(((long)sec % 3600) / 60);
        snprintf(buff, size, "%dh %dm", hours, mins);
    }

    fclose(up_ptr);
}

// network
void get_net_bytes(const char *iface,
                   unsigned long *rx,
                   unsigned long *tx)
{
    (void)iface; // just I didn't want to change funcitons now
    *rx = 0;
    *tx = 0;
    FILE *net_ptr = fopen("/proc/net/dev", "r");
    if (net_ptr == NULL)
    {
        return;
    }

    char line[256];
    fgets(line, sizeof(line), net_ptr);
    fgets(line, sizeof(line), net_ptr);
    while (fgets(line, sizeof(line), net_ptr))
    {
        char *colon = strchr(line, ':');
        if (colon == NULL)
        {
            continue;
        }

        *colon = '\0';
        char *name = line;
        while (*name == ' ')
        {
            name++;
        }

        if (strcmp(name, "lo") == 0)
        {
            continue;
        }

        unsigned long r = 0, t = 0;
        if (sscanf(colon + 1, "%lu %*u %*u %*u %*u %*u %*u %*u %lu", &r, &t) == 2)
        {
            *rx += r;
            *tx += t;
        }
        
    }

    fclose(net_ptr);
}

// UI rendring

void draw_bar(int y, int x, int width, double percentage)
{
    if (percentage < 0.0)
    {
        percentage = 0.0;
    }
    if (percentage > 100.0)
    {
        percentage = 100.0;
    }

    int fileed = (int)((percentage / 100.0) * width);

    int color_pair = 2;
    if (percentage > 80.0)
    {
        color_pair = 4;
    }
    else if (percentage > 50.0)
    {
        color_pair = 3;
    }

    mvaddstr(y, x, "[");
    for (int i = 0; i < width; i++)
    {
        if (i < fileed)
        {
            attron(COLOR_PAIR(color_pair));
            addstr("█");
            attroff(COLOR_PAIR(color_pair));
        }
        else
        {
            addstr("░");
        }
    }
    addstr("]");
}

void draw_header_sec(int y, const char *title, int width)
{
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(y, 0, "|-%s ", title);
    int title_len = strlen(title) + 4;
    for (int i = title_len; i < width; i++)
    {
        addch('-');
    }
    mvprintw(y, width - 1, "-|");
    attroff(COLOR_PAIR(1) | A_BOLD);
}
