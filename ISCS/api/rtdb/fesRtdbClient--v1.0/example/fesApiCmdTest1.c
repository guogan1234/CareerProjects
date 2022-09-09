#include "irtdb_fes_data.h"
#include "irtdb_fes_interface.h"
#include <time.h>
#include <stdio.h>

void test_fep_cmd_processor(const char* cmdValue,irtdb_fep_cmdresult_notifier* notifier,const void* cmdId)
{
    char *cmdResult;
    
    printf("test_fep_cmd_processor begin!\n");
    
    printf("cmdValue : %s \n",cmdValue);
    printf("cmdId : %s \n",cmdId);
    
    // do task 
    cmdResult = "ChangedCmdResult_001";
    
    // notify fesServer
    notifier(cmdResult,cmdId);
    
    printf("test_fep_cmd_processor end!\n");
}

int main()
{
    int iRet = 0;
    time_t curTime = time(NULL);
    char *fesId = "domain_label:station_id:pro_system_label" \
        " rtuid_01   rtuid_02   rtuid_03";
    
    iRet = irtdb_fes_init(fesId);
    if(-1 == iRet)
    {
        printf("irtdb_fes_init fail!\n");
        return -1;
    }
    irtdb_fes_register_commandProcessor(test_fep_cmd_processor); 
    iRet = irtdb_fes_start();
    if(-1 == iRet)
    {
        printf("irtdb_fes_start fail!\n");
        return -1;
    }
    
    printf("start ok , curTime : %d\n",(int)curTime); 

    while(1) sleep(1);
    irtdb_fes_unregister_commandProcessor();
    irtdb_fes_stop();
    printf("ok!\n");
    return 0;
}
