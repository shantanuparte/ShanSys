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
    
    struct Process_info *process = process_directory(); //everthing goes in process array
    struct Cpu_jiffies *cpu_jiffie1 = get_cpu_jiffies();

    for (int i = 0; i < 10; i++)
    {
        process_jiffies_process_info(process[i].pid,i);
    }


    
    
    nanosleep(&ts, NULL); // sleep for half second herec


    return 0;
}
