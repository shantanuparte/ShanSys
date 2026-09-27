#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ram.h"


long int* get_ram_stats(){

    FILE *ram_ptr = fopen("/proc/meminfo", "r");

    if (ram_ptr == NULL)
    {
        fprintf(stderr, "Error: In file opening of ram");
        return NULL;
    }
    

    char ram_buff[1024];
    long int ram_stats[3];
    size_t s = sizeof(long int);

    int parsed = 0, total_parsed = 0;
    for(int i = 0; i < 3; i++){
        fgets(ram_buff,sizeof(ram_buff), ram_ptr);
        parsed = sscanf(ram_buff, "%*s  %ld",&ram_stats[i]);
        total_parsed += parsed;
    }

    if (total_parsed != 3)
    {
        fclose(ram_ptr);
        return NULL;
    }

    long int* dy_ram_stats_array = malloc(s * 4);

    for (int i = 0; i < 3; i++)
    {
        printf("%ld\n",ram_stats[i]);
        dy_ram_stats_array[i] = ram_stats[i];
    }

    get_percentage_of_ram(dy_ram_stats_array);


    fclose(ram_ptr);
    
    return dy_ram_stats_array;
}


void get_percentage_of_ram(long int * array){

    long int total,free_space,percentage;
    total = array[0];
    free_space = array[1];

    percentage = ((double)free_space/total) * 100;
    array[3] = percentage;

}
