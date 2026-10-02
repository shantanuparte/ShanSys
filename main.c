#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "ram.h"
#include "cpu.h"
#include "process.h"
#include "other.h"
#include <locale.h>

int main(int args, char *argv[]) // Sleep should be added in main not in other file
{
    setlocale(LC_ALL, "");
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
            sizeof(uptime));

        unsigned long rx = 0;
        unsigned long tx = 0;

        get_net_bytes(
            "wlp2s0",
            &rx,
            &tx);

        clear();
        int box_width = 70;

        attron(COLOR_PAIR(1) | A_BOLD);
        draw_header_sec(0, "ShanSys System Monitor", box_width);
        attroff(COLOR_PAIR(1) | A_BOLD);

        mvprintw(1, 2, "CPU Usage: %d%%", percentage);
        draw_bar(1, 20, 20, percentage);
        mvprintw(2, 2, "Averge Load: %.2f", load);
        mvprintw(3, 2, "Uptime: %s", uptime);
        if (battery >= 0)
        {
            mvprintw(4,2,"Battry: %d%%",battery);
        }else{
            mvprintw(4,2,"Battry: Not deltected");
        }
        

        if (stats != NULL)
        {
            mvprintw(5, 2, "RAM Usage: %ld%%", stats[3]);
            draw_bar(5, 20, 20, (double)stats[3]);
            free(stats);
        }

        // network
        draw_header_sec(7, "Network", box_width);

        mvprintw(8, 4, "RX: %.2f MB", rx / (1024.0 * 1024.0));
        mvprintw(9, 4, "TX: %.2f MB", tx / (1024.0 * 1024.0));

        // process table
        draw_header_sec(11, "Process", box_width);

        attron(A_BOLD);
        mvprintw(12, 2, "%-8s %-25s %-8s %-12s %-10s", "PID", "NAME", "STATE", "MEM(MB)", "CPU(%%)");

        attroff(A_BOLD);

        for (int i = 0; i < 10; i++)
        {
            double process_cpu_percent = calculate_process_percentages(&cp_arr1[i], &cpu_arr2[i], total_sys_jiff);
            long memory = process[i].memory / 1024;
            mvprintw(14 + i, 2, "%-8d %-25s %-8c %-12ld %-10.2f", process[i].pid, process[i].name, process[i].state, memory, process_cpu_percent);
        }

        attron(COLOR_PAIR(1) | A_BOLD);
        mvaddch(25, 0, '|');
        for (int i = 0; i < box_width; i++)
        {
            addch('_');
        }
        addch('|');

        attroff(COLOR_PAIR(1) | A_BOLD);
        
        refresh();

        free(cpu_jiffies1);
        free(cpu_jiffies2);
        free(cpu_arr2);
        free(process);
    }

    free(cp_arr1);
    endwin();

    return 0;
}
