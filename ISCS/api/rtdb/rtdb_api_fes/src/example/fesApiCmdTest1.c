#include "irtdb_fes_data.h"
#include "irtdb_fes_interface.h"
#include <time.h>
#include <stdio.h>

irtdb_fep_cmdresult_notifier* g_notifier;
char *gCmdResult;
char *gCmdId;
int gTaskFlag = 0;

void test_fep_cmd_processor(const char* cmdValue,irtdb_fep_cmdresult_notifier* notifier,const void* cmdId)
{
    char *cmdResult;
    
    printf("test_fep_cmd_processor begin!\n");
    
    printf("cmdValue : %s \n",cmdValue);
    printf("cmdId : %s \n",cmdId);
    
    // do task 
    cmdResult = "ChangedCmdResult_001";
    
    g_notifier = notifier;
    gCmdResult = calloc(strlen(cmdResult) + 1,0);
    if(gCmdResult) memcpy(gCmdResult,cmdResult,strlen(cmdResult));
    
    gCmdId = cmdId;
   
    printf("test_fep_cmd_processor end!\n");
    gTaskFlag = 1;
	if(gCmdResult) free(gCmdResult);
}

int main()
{
    int iRet = 0;
    long long curTime = time(NULL) * 1000000;
    char *fesId = "fz2cld:fz2cld:BAS:group1:rtu1 fz2cld:fz2cld:BAS:group1:rtu2";
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
    
    printf("start ok , curTime : %ld \n",curTime); 
        
    while(1)
    {
		sleep(1);
        if(!gTaskFlag)
        {
            //printf("wait notify result!\n");
            continue;
        }
        g_notifier(gCmdResult,gCmdId);
        gTaskFlag = 0;
		break;
    } 
    
    irtdb_fes_unregister_commandProcessor();
    irtdb_fes_stop();
	irtdb_fes_uninit();
	printf("ok!\n");
    return 0;
}
