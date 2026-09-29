#include "process.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>

void get_process_info(int pid){

    char path[256];
    snprintf(path,sizeof(path),"/path/%d/stat");

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
        printf("Error opening directory");
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_DIR && is_numric(entry->d_name))
        {
            int pid = atoi(entry->d_name);
            printf("DIRECTORY: %s\n", entry->d_name);
        }
        
        
    }

    if (closedir(directory) == -1)
    {
        printf("Error: closing directory.\n");
        return;
    }
}
