#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "ram.h"
#include "cpu.h"
#include "process.h"
#include "other.h"

int main(int args, char *argv[]) // Sleep should be added in main not in other file
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 500000000;

    initscr();
    start_color();
    noecho();
    cbreak();
    curs_set(0);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    struct Process_jiffies *cp_arr1 = initilize_cpu_array();

    if (cp_arr1 == NULL)
    {
        endwin();
        fprintf(stderr, "Failed to initialize CPU array\n");
        return 1;
    }

    while (1)
    {
        long int *stats = get_ram_stats();

        struct Cpu_jiffies *cpu_jiffies1 = get_cpu_jiffies();
        if (cpu_jiffies1 == NULL)
        {
            break;
        }

        struct Process_info *process = process_directory();

        if (process == NULL)
        {
            free(cpu_jiffies1);
            break;
        }

        for (int i = 0; i < 10; i++)
        {
            process_jiffies_process_info(process[i].pid, i, cp_arr1);
        }

        nanosleep(&ts, NULL);

        struct Cpu_jiffies *cpu_jiffies2 = get_cpu_jiffies();

        if (cpu_jiffies2 == NULL)
        {
            free(cpu_jiffies1);
            free(process);
            break;
        }

        struct Process_jiffies *cpu_arr2 = initilize_cpu_array();

        if (cpu_arr2 == NULL)
        {
            free(cpu_jiffies1);
            free(cpu_jiffies2);
            free(process);
            break;
        }

        for (int i = 0; i < 10; i++)
        {
            process_jiffies_process_info(process[i].pid, i, cpu_arr2);
        }

        int percentage = get_cpu_percentage(cpu_jiffies1, cpu_jiffies2);

        unsigned long total_sys_jiff = cpu_jiffies2->total - cpu_jiffies1->total;

        int battery = get_battery_percentage();

        double load = get_load_avg();

        char uptime[64] = "N/A";

        get_uptime(
            uptime,
            sizeof(uptime)
        );

        unsigned long rx = 0;
        unsigned long tx = 0;

        get_net_bytes(
            "wlp2s0",
            &rx,
            &tx
        );

        clear();
        int box_width = 70;

        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(0,0,"ShanSys System Monitor");
        attroff(COLOR_PAIR(1) | A_BOLD);

        mvprintw(1, 2, "CPU Usage: %d%%",percentage);
        mvprintw(2,2, "Averge Load: %.2f",load);
        mvprintw(3,2, "Uptime: %s",uptime);
        mvprintw(4,2, "Battery: %d%%",battery);

        if (stats != NULL)
        {
            mvprintw(4,2, "RAM Usage: %ld%%",stats[3]);
            free(stats);
        }

        
        

    }

    return 0;
}
