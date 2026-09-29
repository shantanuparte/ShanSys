#include "process.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>

struct Process_info process[1000];
int process_count = 0;

void get_process_info(int pid)
{
    struct Process_info p;
    char path[256];
    char line[256];
    snprintf(path, sizeof(path), "/proc/%d/status",pid); // I will not get cpu% for this so have to make something in helper. remember

    FILE *file_ptr = fopen(path, "r");

    if (file_ptr == NULL)
    {
        fprintf(stderr, "Error: Opning file");
        return;
    }

    p.pid = pid;

    while (fgets(line, sizeof(line), file_ptr) != NULL)
    {
        if (sscanf(line, "Name:\t%s", p.name))
        {
            continue;
        }
        if (sscanf(line, "State:\t%c", &p.state))
        {
            continue;
        }
        if (sscanf(line, "VmRSS:\t%ld", &p.memory))
        {
            continue;
        }
    }


    fclose(file_ptr);
    process[process_count] = p;
    process_count++;
}

void store_array()
{
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

void process_directory()
{

    DIR *directory;
    struct dirent *entry;

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

    for (int i = 0; i < process_count; i++)
    {
        printf("\nName: %s\n",process[i].name);
        printf("Pid: %d\n",process[i].pid);
        printf("Memory: %ld\n",process[i].memory);
        printf("State: %c\n",process[i].state);
    }
    

}
