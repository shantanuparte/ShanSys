#ifndef PROCESS_INFO
#define PROCESS_INFO

struct Process_info
{
    char name[100];
    unsigned long int memory;
    int pid;
    // int cpu;  wanna add later 
    char state;
};

void process_directory();
int is_numric(const char *s);

#endif