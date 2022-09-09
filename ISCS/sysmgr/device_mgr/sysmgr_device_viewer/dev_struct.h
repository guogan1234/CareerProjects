#ifndef DEV_STRUCT_H
#define DEV_STRUCT_H
typedef struct process_data
{
    char *pid;
    char *cpu;
    char *mem;
    char *command;
}process;

typedef struct cpu_data
{
    char *name;
    char *user;
    char *nice;
    char *system;
    char *idle;
    char *iowait;
    char *irq;
    char *softirq;
    //char *rate;
}cpu;

typedef struct mem_data
{
    char *name;
    char *size;
}mem;

typedef struct phy_data
{
    char *filesystem;
    char *size;
    char *used;
    char *avail;
    char *use_percent;
    char *mounted;
}phy;

#endif // DEV_STRUCT_H



