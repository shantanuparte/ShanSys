#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

void get_cpu_stas()
{
}

void get_cpu_basic_info()
{
    FILE *cpu_info_ptr = fopen("/proc/cpuinfo", "r");
    int stop = 0;
    if (cpu_info_ptr == NULL)
    {
        perror("Error opening file: cpuinfo");
        return;
    }
    char cpu_info_buff[1024];
    u_int8_t cores = 0;
    while (fgets(cpu_info_buff, sizeof(cpu_info_buff), cpu_info_ptr) || stop == 5)
    {

        //model name	CPU name
        // vendor_id	CPU manufacturer
        // cpu cores	Physical cores
        // siblings	Logical CPUs / threads
        // cpu MHz	Current frequency
        // flags	Optional — CPU capabilities
        if (sscanf(cpu_info_buff,"cpu cores: %s",&cores))
        {
            stop++;
            continue;
        }
        
        
    }

    printf("%d\n",cores);
}

void get_cpu_percentage()
{

    FILE *cpu_stats_ptr = fopen("/proc/stat", "r");

    if (cpu_stats_ptr == NULL)
    {
        perror("Error opening file: stats");
        return;
    }

    char cpu_stats_buff[1024];

    fgets(cpu_stats_buff, sizeof(cpu_stats_buff), cpu_stats_ptr);
    printf("%s", cpu_stats_buff);
}
