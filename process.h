#ifndef PROCESS_INFO
#define PROCESS_INFO

#define PROCESS_CAPACITY 750

struct Process_info
{
    char name[100];
    unsigned long int memory;
    int pid;
    // int cpu;  wanna add later
    char state;
};

struct Process_jiffies
{
    unsigned long int utime;
    unsigned long int stime;
};
double calculate_process_percentages(struct Process_jiffies *p1,
                                     struct Process_jiffies *p2,
                                     unsigned long total_time);
void process_jiffies_process_info(int pid, int i, struct Process_jiffies *cpu_array);
struct Process_jiffies *initilize_cpu_array();
int compare_mem(const void *a, const void *b);
void sort_array(struct Process_info *process);
void get_process_info(int pid);
int is_numric(const char *s);
struct Process_info *initilize_process_array();
struct Process_info *process_directory();

#endif