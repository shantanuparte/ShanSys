#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ram.h"

long int *get_ram_stats()
{

    FILE *ram_ptr = fopen("/proc/meminfo", "r");

    if (ram_ptr == NULL)
    {
        fprintf(stderr, "Error: In file opening of ram");
        return NULL;
    }

    char ram_buff[1024];
    long int ram_stats[3];
    size_t s = sizeof(long int);
    long int total = 0;
    long int free_ram = 0;
    long int avaliable = 0;
    int stop = 0;
    int parsed = 0, total_parsed = 0;

    while (fgets(ram_buff, sizeof(ram_buff), ram_ptr) || stop != 3)
    {
        if (sscanf(ram_buff, "MemTotal: %ld", &total))
        {  
            stop++;
            continue;
        }
        if (sscanf(ram_buff, "MemFree: %ld", &free_ram))
        {
            stop++;
            continue;
        }
        if (sscanf(ram_buff, "MemAvailable: %ld", &avaliable))
        {
            stop++;
            continue;
        }
    }
    

    

    long int *dy_ram_stats_array = malloc(s * 4);

    dy_ram_stats_array[0] = total;
    dy_ram_stats_array[1] = free_ram;
    dy_ram_stats_array[2] = avaliable;
    get_percentage_of_ram(dy_ram_stats_array);
    
    

    fclose(ram_ptr);

    return dy_ram_stats_array;
}

void get_percentage_of_ram(long int *array)
{

    long int total, free_space, percentage;
    total = array[0];
    free_space = array[1];

    percentage = ((double)free_space / total) * 100;
    array[3] = percentage;
}
