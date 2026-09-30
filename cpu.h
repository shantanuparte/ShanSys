#ifndef CPU_USAGE
#define CPU_USAGE

struct Cpu_Entries
{
    // due to structure padding write from max sizes to lower sized not ai
    char model[100];
    char vendor[100];
    float freq;
    unsigned int cores;
    unsigned int seb;
};

struct Cpu_jiffies
{
    unsigned long int user;
    unsigned long int nice;
    unsigned long int system;
    unsigned long int idel;
    unsigned long int iowait;
    unsigned long int irq;
    unsigned long int softirq;
    unsigned long int steal;
};

struct Cpu_Entries *get_cpu_basic_info();
struct Cpu_jiffies *get_cpu_jiffies();
void get_cpu_stas();
long int get_cpu_percentage(struct Cpu_jiffies *cj1, struct Cpu_jiffies *cj2);

#endif