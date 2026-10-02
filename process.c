#include "process.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process_info *process = NULL;
int process_count = 0;
int process_capacity = PROCESS_CAPACITY;

double calculate_process_percentages(struct Process_jiffies *p1,
                                     struct Process_jiffies *p2,
                                     unsigned long total_time)
{
    if (total_time == 0)
    {
        return 0.0;
    }

    unsigned long process_utime_diff = p2->utime - p1->utime;
    unsigned long process_stime_diff = p2->stime - p1->stime;
    unsigned long process_total_diff = process_utime_diff + process_stime_diff;

    double percentage = ((double)process_total_diff / (double)total_time) * 100;
    return percentage;
}

void process_jiffies_process_info(int pid, int i, struct Process_jiffies *cpu_array)
{
    char path[1024];
    char buff[1024];

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    struct Process_jiffies p = {0};
    FILE *file_ptr = fopen(path, "r");
    if (file_ptr && fgets(buff, sizeof(buff), file_ptr))
    {
        char *after_comm = strrchr(buff, ')');
        if (after_comm)
        {
            sscanf(after_comm + 1, " %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu", &p.utime, &p.stime);
        }
        fclose(file_ptr);
    }
    cpu_array[i] = p;
}

struct Process_jiffies *initilize_cpu_array()
{
    struct Process_jiffies *cpu_array = NULL;
    int cpu_array_capacity = 10;
    cpu_array = malloc(sizeof(struct Process_jiffies) * cpu_array_capacity);
    if (cpu_array == NULL)
    {
        fprintf(stderr, "Error: Initilization of cpu_array\n");
        return NULL;
    }
    return cpu_array;
}

struct Process_info *initilize_process_array()
{

    process = malloc(process_capacity * sizeof(struct Process_info));

    if (process == NULL)
    {
        fprintf(stderr, "Error: Malloc allocation form process lol");
        return NULL;
    }

    return process;
}

int compare_mem(const void *a, const void *b)
{
    const struct Process_info *p1 = a;
    const struct Process_info *p2 = b;

    if (p1->memory < p2->memory)
        return 1;
    if (p1->memory > p2->memory)
        return -1;

    return 0;
}

void sort_array(struct Process_info *process)
{

    qsort(process, process_count, sizeof(struct Process_info), compare_mem);
}

void get_process_info(int pid)
{
    struct Process_info p = {0};
    char path[256];
    char line[256];
    snprintf(path, sizeof(path), "/proc/%d/status",
             pid); // I will not get cpu% for this so have to make something in
                   // helper. remember

    FILE *file_ptr = fopen(path, "r");

    if (file_ptr == NULL)
    {
        return;
    }

    p.pid = pid;

    while (fgets(line, sizeof(line), file_ptr) != NULL)
    {
        if (sscanf(line, "Name: %255s", p.name) == 1)
        {
            continue;
        }
        if (sscanf(line, "State: %c", &p.state) == 1)
        {
            continue;
        }
        if (sscanf(line, "VmRSS: %ld", &p.memory) == 1)
        {
            continue;
        }
    }

    fclose(file_ptr);

    process[process_count] = p;
    process_count++;
    if (process_count >= process_capacity)
    {
        process_capacity *= 2;
        struct Process_info *temp =
            realloc(process, process_capacity * sizeof(struct Process_info));
        if (temp == NULL)
        {
            fprintf(stderr, "Error: Relloc\n");
            return;
        }
        process = temp;
    }
}

int is_numric(const char *s)
{
    while (*s)
    {
        if (!isdigit(*s))
        {
            return 0;
        }

        s++;
    }
    return 1;
}

struct Process_info *process_directory() // Main function in this
{

    process_count = 0;
    process_capacity = PROCESS_CAPACITY;

    process = initilize_process_array();
    DIR *directory;
    struct dirent *entry;

    directory = opendir("/proc");

    if (directory == NULL)
    {
        fprintf(stderr, "Error opening directory");
        return NULL;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_DIR && is_numric(entry->d_name))
        {
            int pid = atoi(entry->d_name);
            get_process_info(pid);
        }
    }

    if (closedir(directory) == -1)
    {
        fprintf(stderr, "Error: closing directory.\n");
        return NULL;
    }
    sort_array(process);

    return process;
}
