#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>

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

struct Cpu_jiffies *get_cpu_jiffies()
{
    struct Cpu_jiffies *cj = malloc(sizeof(struct Cpu_jiffies));
    FILE *cpu_stats_ptr = fopen("/proc/stat", "r");

    if (cpu_stats_ptr == NULL)
    {
        perror("Error opening file: stats");
        return NULL;
    }

    char cpu_stats_buff[1024];

    fgets(cpu_stats_buff, sizeof(cpu_stats_buff), cpu_stats_ptr);
    // printf("%s", cpu_stats_buff);

    sscanf(cpu_stats_buff, "cpu %ld %ld %ld %ld %ld %ld %ld %ld", &cj->user, &cj->nice, &cj->system, &cj->idel, &cj->iowait, &cj->irq, &cj->softirq, &cj->steal);

    return cj;
}

long int get_cpu_percentage(struct Cpu_jiffies *cj1, struct Cpu_jiffies *cj2)
{

    unsigned long int Idle_time1 = cj1->idel + cj1->iowait;
    unsigned long int Active_time1 = cj1->user + cj1->nice + cj1->system + cj1->irq + cj1->softirq + cj1->steal;
    unsigned long int Total_time1 = Idle_time1 + Active_time1;

    unsigned long int Idle_time2 = cj2->idel + cj2->iowait;
    unsigned long int Active_time2 = cj2->user + cj2->nice + cj2->system + cj2->irq + cj2->softirq + cj2->steal;
    unsigned long int Total_time2 = Idle_time2 + Active_time2;

    unsigned long int delta_idle = Idle_time2 - Idle_time1;
    unsigned long int delta_total = Total_time2 - Total_time1;

    if (delta_total == 0)
    {
        free(cj1);
        free(cj2);
        return 0;
    }

    unsigned long int cpu_percentage = ((delta_total - delta_idle) * 100) / delta_total;
    free(cj1);
    free(cj2);

    return cpu_percentage;
}

void get_cpu_stas()
{
    struct Cpu_Entries *ent1;
    unsigned long int percentage;

    ent1 = get_cpu_basic_info();

    printf("Model name: %s\n", ent1->model);
    printf("Cores: %d\n", ent1->cores);
    printf("Freq: %.2f MHz\n", ent1->freq);
    printf("Vendor: %s\n", ent1->vendor);
    printf("Logical cores: %d\n\n", ent1->seb);

}
