#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>



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
    int cores = 0;
    float freq = 0;
    int sib = 0;
    char vendor[100];
    char model_name[100];
    while (fgets(cpu_info_buff, sizeof(cpu_info_buff), cpu_info_ptr))
    {

        //model name	CPU name
        // vendor_id	CPU manufacturer
        // cpu cores	Physical cores
        // siblings	Logical CPUs / threads
        // cpu MHz	Current frequency
        // flags	Optional — CPU capabilities
        if (sscanf(cpu_info_buff,"cpu cores : %d",&cores))
        {
            stop++;
            continue;
        }
        if (sscanf(cpu_info_buff,"cpu MHz : %f",&freq))
        {
            stop++;
            continue;
        }
        
        if (sscanf(cpu_info_buff,"vendor_id : %s",vendor))
        {
            stop++;
            continue;
        }

        if (sscanf(cpu_info_buff,"siblings : %d",&sib))
        {
            stop++;
            continue;
        }
        
        if (sscanf(cpu_info_buff,"model name : %s",model_name))
        {
            stop++;
            continue;
        }
        

        
        
        
    }

    printf("Model name: %s\n",model_name);
    printf("Cores: %d\n",cores);
    printf("Freq: %.2f MHz\n",freq);
    printf("Vendor: %s\n",vendor);
    printf("Logical cores: %d",sib);
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


void get_cpu_stas()
{
    get_cpu_basic_info();

}