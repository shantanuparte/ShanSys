#ifndef PROCESS_INFO
#define PROCESS_INFO

struct Info
{
    char name[100];
    int cpu;
    int memory;
    int pid;
};

void process_directory();
int is_numric(const char *s);

#endif