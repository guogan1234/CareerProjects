#include "work.h"
#include "c_list.h"
#include "work_detail.h"
#include "work_args.h"
#include <signal.h>
#include <unistd.h>

#include <stdio.h>

int interval = 0;

void timeout(int signum){
    printf("timeout...\n");
    if(SIGALRM == signum){
        alarm(interval);
    }

    return;
}

void* t_run(void *param){
    printf("t_run...\n");
    WorkThreadArg* args = (WorkThreadArg*)param;
    interval = args->interval;

    printf("before getCfgIps...\n");
    c_list connInfoList;
    __c_list(&connInfoList,NULL);
    getCfgIps(&connInfoList);
    printf("after getCfgIps...\n");

    printf("before alarm...\n");
    signal(SIGALRM,timeout);
    alarm(args->interval);
    printf("after alarm...\n");

    for(;;);
    printf("t_run exit!\n");
    return NULL;
}
