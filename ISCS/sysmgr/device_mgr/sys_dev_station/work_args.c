#include "work_args.h"

#include <stdio.h>

int getWorkInterval(){
    return 3;
}

int getWorkArgs(WorkThreadArg* workArgs){
    if(workArgs == NULL){
        return -1;
    }

    workArgs->interval = getWorkInterval();

    return 0;
}
