#include "master_manager.h"
#include "work_controller.h"

#include <stdio.h>

//int bWork = 0;

int main(){
    if(isMaster()){
        startWork();
    }else {
        stopWork();
    }

    while(1){

    }
    printf("app end!\n");
    return 0;
}
