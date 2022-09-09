#include "work_detail.h"

#include <stdio.h>

int getCfgIps(c_list* list){
    if(NULL == list){
        printf("args:[%s][%d]\n",__FILE__,__LINE__);
        return -1;
    }

    char* ip_1 = "127.0.0.1";
    char* ip_2 = "192.168.1.111";
    c_list_push_back(list,ip_1);
    c_list_push_back(list,ip_2);
    return 0;
}
