#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ram.h"


void get_ram_stats(){

    FILE *ram_ptr = fopen("/proc/meminfo", "r");

    char ram_buff[1024];
    long double ram_stats[3];

    int parsed = 0, total_parsed = 0;
    for(int i = 0; i < 3; i++){
        fgets(ram_buff,sizeof(ram_buff), ram_ptr);
        parsed = sscanf(ram_buff, "%*s  %ld",ram_stats[i]);
        total_parsed += parsed;
    }

    if (total_parsed != 3)
    {
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        printf("%ld\n",ram_stats[i]);
    }


    
    fclose(ram_ptr);

}

