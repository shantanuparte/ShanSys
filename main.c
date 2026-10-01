#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ram.h"
#include "cpu.h"
#include "process.h"

int main(int args, char *argv[]) // Sleep should be added in main not in other file
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 500000000;

    struct Process_jiffies *cp_arr1 = initilize_cpu_array();

    printf("---WELCOME TO SHANSYS---\n\n");

    long int *stats = get_ram_stats();
    if (stats != NULL)
    {
        printf("Ram Usage: %ld%%\n", stats[3]);
        free(stats); // Prevent memory leak
    }

    get_cpu_stas();

    struct Process_info *process = process_directory(); // everthing goes in process array
    struct Cpu_jiffies *cpu_jiffie1 = get_cpu_jiffies();

    for (int i = 0; i < 10; i++)
    {
        process_jiffies_process_info(process[i].pid, i, cp_arr1);
    }

    nanosleep(&ts, NULL); // sleep for half second here

    struct Cpu_jiffies *cpu_jiffie2 = get_cpu_jiffies();
    struct Process_jiffies *cpu_arr2 = initilize_cpu_array();

    for (int i = 0; i < 10; i++)
    {
        process_jiffies_process_info(process[i].pid, i, cpu_arr2);
    }

    int percentage = get_cpu_percentage(cpu_jiffie1, cpu_jiffie2);

    printf("Percentage: %d\n", percentage);

    unsigned long total_sys_jiff = cpu_jiffie2->total - cpu_jiffie1->total;

    printf("%-8s %-25s %-8s %-12s %-10s\n", "PID", "NAME", "STATE", "MEM(KB)", "CPU(%)");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < 10 && i < 10; i++)
    {
        double process_cpu_percent = calculate_process_percentages(&cp_arr1[i], &cpu_arr2[i], total_sys_jiff);

        printf("%-8d %-25s %-8c %-12ld %-10.2f\n",
               process[i].pid,
               process[i].name,
               process[i].state,
               process[i].memory,
               process_cpu_percent);
    }

    free(cp_arr1);
    free(cpu_arr2);
    free(cpu_jiffie1);
    free(cpu_jiffie2);
    free(process);


        return 0;
}
