#ifndef SYSCONFIN_H
#define SYSCONFIN_H

struct SysDomain{
    int domain_id;
    char* name;
    char* name_ch;
    int domain_type;
};
typedef struct SysDomain SysDomain;

struct SysNode{
    int type;
    char* name;
    char* name_ch;
    int status;
    int nic_num;
    char* nic_1_name;
    char* nic_1_netmask;
    char* nic_1_address;
    char* nic_2_name;
    char* nic_2_netmask;
    char* nic_2_address;
    int net_status;
    char* dev;
    char* os;
    char* location;
    int domain_id;
};
typedef struct SysNode SysNode;

struct SysProcess{
    char* alias;
    char* command;
    int instance_num;
    int start_type;
    int autorun;
    int cycle_start;
    int cycle_period;
    int offline_run;
    int duty_run;
    int standby_run;
    int run_order;
    int report_type;
    char* file_path;
    char* para;
    char* description;
    int run_style;
    int proc_type;
};
typedef struct SysProcess SysProcess;

struct SysDomainProc{
    char* process_sub;
    int domain_id;
};
typedef struct SysDomainProc SysDomainProc;

#endif // SYSCONFIN_H
