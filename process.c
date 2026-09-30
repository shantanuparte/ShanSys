#include "process.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>

struct Process_info *process = NULL;
int process_count = 0;
int process_capacity = PROCESS_CAPACITY;

struct Process_jiffies *cpu_array = NULL;
int cpu_array_capacity = 10;

int process_jiffies_func(int pid)
{
    char path[256];
    float percent = 0;
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    struct Process_jiffies *p = malloc(sizeof(struct Process_jiffies));
    FILE *file_ptr = fopen(path, "r");

    

}

void initilize_cpu_array()
{
    cpu_array = malloc(sizeof(struct Process_jiffies) * cpu_array_capacity);
    if (cpu_array == NULL)
    {
        fprintf(stderr,"Error: Initilization of cpu_array\n");
        return;
    }
    
}

void initilize_process_array()
{

    process = malloc(process_capacity * sizeof(struct Process_info));

    if (process == NULL)
    {
        fprintf(stderr, "Error: Malloc allocation form process lol");
        return;
    }
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

void sort_array()
{

    qsort(process, process_count, sizeof(struct Process_info), compare_mem);
}

void get_process_info(int pid)
{
    struct Process_info p = {0};
    char path[256];
    char line[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid); // I will not get cpu% for this so have to make something in helper. remember

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
    if (process_count >= process_capacity)
    {
        process_capacity *= 2;
        struct Process_info *temp = realloc(process, process_capacity * sizeof(struct Process_info));
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

void process_directory() // Main function in this
{

    DIR *directory;
    struct dirent *entry;
    initilize_process_array();
    directory = opendir("/proc");

    if (directory == NULL)
    {
        fprintf(stderr, "Error opening directory");
        return;
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
        return;
    }
    sort_array();

    printf("%-6s %-20s %-12s %-6s\n", "PID", "NAME", "MEM (MB)", "STATE");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < process_count; i++)
    {
        // Converting KB to MB for cleaner readability
        double mem_mb = process[i].memory / 1024.0;

        printf("%-6d %-20.20s %-12.2f %-6c\n",
               process[i].pid,
               process[i].name,
               mem_mb,
               process[i].state);
    }
}
