#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ram.h"
#include "cpu.h"
#include "process.h"



int main(int args, char *argv[])//Sleep should be added in main not in other file
{

    // printf("---WELCOME TO SHANSYS---\n\n");

    // long int *stats = get_ram_stats();
    // if (stats != NULL)
    // {
    //     printf("Ram Usage: %ld%%\n", stats[3]);
    //     free(stats); // Prevent memory leak
    // }

    // get_cpu_stas();

    process_directory();
    

    return 0;
}
