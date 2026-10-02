#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

// Battry

int get_battery_percentage()
{
    int cap = -1;
    FILE *battry_ptr = fopen("/sys/class/power_supply/BAT0/capacity", "r");

    if (battry_ptr == NULL)
    {
        fscanf(battry_ptr, "%d", &cap);
    }

    fclose(battry_ptr);
    return cap;
}

// avg load

void get_load_avg()
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


//network 


