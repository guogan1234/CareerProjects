#include "work_controller.h"
#include <pthread.h>
#include <stdlib.h>
#include "work.h"
#include "parser.h"
#include "work_args.h"

#include <stdio.h>

int startWork(){
    printf("startWork...\n");
    WorkThreadArg* workArgs = (WorkThreadArg*)malloc(sizeof(WorkThreadArg));
    getWorkArgs(workArgs);

    pthread_t t;
    pthread_create(&t,NULL,t_run,workArgs);
    pthread_detach(t);

    pthread_t tParser;
    pthread_create(&tParser,NULL,t_parse,NULL);
    pthread_detach(tParser);

    return 0;
}

int stopWork(){

    return 0;
}
