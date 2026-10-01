#include "process.h"
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

struct Process_info *process = NULL;
int process_count = 0;
int process_capacity = PROCESS_CAPACITY;

struct Process_jiffies *cpu_array = NULL;
int cpu_array_capacity = 10;

void process_jiffies_process_info(int pid, int i)
{
    char path[1024];
    char buff[1024];
    float percent = 0;

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    struct Process_jiffies p = {0};
    FILE *file_ptr = fopen(path, "r");
    if (file_ptr && fgets(buff, sizeof(buff), file_ptr))
{
    sscanf(buff, "%*d %*s %*c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %lu", &p.utime, &p.stime);
    fclose(file_ptr);
}


    cpu_array[i] = p;

}

struct Process_jiffies *initilize_cpu_array()
{
    cpu_array = malloc(sizeof(struct Process_jiffies) * cpu_array_capacity);
    if (cpu_array == NULL)
    {
        fprintf(stderr, "Error: Initilization of cpu_array\n");
        return NULL;
    }
    return cpu_array;
}

struct Process_info* initilize_process_array()
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
