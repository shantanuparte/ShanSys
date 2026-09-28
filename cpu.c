#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

struct Cpu_Entries *get_cpu_basic_info()
{
    FILE *cpu_info_ptr = fopen("/proc/cpuinfo", "r");
    int stop = 0;

    struct Cpu_Entries *ce = malloc(sizeof(struct Cpu_Entries));

    if (cpu_info_ptr == NULL)
    {
        perror("Error opening file: cpuinfo");
        return NULL;
    }
    char cpu_info_buff[1024];
    unsigned int cores = 0;
    float freq = 0;
    unsigned int sib = 0;
    char vendor[100];
    char model_name[100];
    while (fgets(cpu_info_buff, sizeof(cpu_info_buff), cpu_info_ptr))
    {

        // model name	CPU name
        //  vendor_id	CPU manufacturer
        //  cpu cores	Physical cores
        //  siblings	Logical CPUs / threads
        //  cpu MHz	Current frequency
        //  flags	Optional — CPU capabilities
        if (sscanf(cpu_info_buff, "cpu cores : %d", &cores))
        {
            stop++;
            continue;
        }
        if (sscanf(cpu_info_buff, "cpu MHz : %f", &freq))
        {
            stop++;
            continue;
        }

        if (sscanf(cpu_info_buff, "vendor_id : %s", vendor))
        {
            stop++;
            continue;
        }

        if (sscanf(cpu_info_buff, "siblings : %d", &sib))
        {
            stop++;
            continue;
        }

        if (sscanf(cpu_info_buff, "model name\t: %[^\n]", model_name))
        {
            stop++;
            continue;
        }
    }

    ce->cores = cores;
    ce->freq = freq;
    ce->seb = sib;
    strcpy(ce->model, model_name);
    strcpy(ce->vendor, vendor);
    return ce;
}

void get_cpu_percentage()
{
    struct Cpu_jiffies cj = {0};

    FILE *cpu_stats_ptr = fopen("/proc/stat", "r");

    if (cpu_stats_ptr == NULL)
    {
        perror("Error opening file: stats");
        return;
    }

    char cpu_stats_buff[1024];

    fgets(cpu_stats_buff, sizeof(cpu_stats_buff), cpu_stats_ptr);
    printf("%s", cpu_stats_buff);

    sscanf(cpu_stats_buff, "cpu %ld %ld %ld %ld %ld %ld %ld %ld", &cj.user, &cj.nice, &cj.system, &cj.idel, &cj.iowait, &cj.irq, &cj.softirq, &cj.steal);

    printf("\n\nUser: %ld\n", cj.user);
    printf("Nice: %ld\n", cj.nice);
    printf("System: %ld\n", cj.system);
    printf("Idle: %ld\n", cj.idel);
    printf("IoWait: %ld\n", cj.iowait);
    printf("Irq: %ld\n", cj.irq);
    printf("SoftIrq: %ld\n", cj.softirq);
    printf("steal: %ld\n", cj.steal);
}

void get_cpu_stas()
{
    struct Cpu_Entries *ent;

    ent = get_cpu_basic_info();

    printf("Model name: %s\n", ent->model);
    printf("Cores: %d\n", ent->cores);
    printf("Freq: %.2f MHz\n", ent->freq);
    printf("Vendor: %s\n", ent->vendor);
    printf("Logical cores: %d", ent->seb);

    get_cpu_percentage();
}
