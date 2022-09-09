#include "irtdb_fes_data.h"
#include "irtdb_fes_interface.h"
#include <time.h>
#include <stdio.h>

int test_upload_soe(long long curTime)
{
    int iRet = -1;

    iRet = irtdb_fes_upload_soe(
        "fz2cld:fz2cld:BAS:UPSA:SRDY",
        //"fz2cld:fz2cld:PSCADA:100:YFFG",
        "value_01","timeLabel_001",curTime
    );
    if(0 == iRet)
        printf("irtdb_fes_upload_soe ok!\n");
    else
        printf("irtdb_fes_upload_soe fail!\n");
    
}

int test_multiupload_soe(long long curTime)
{
    int iRet = -1;
    irtdb_fep_soe **soes = NULL;
    irtdb_fep_soe *soe = NULL;
    int i = 0,soeSize = 0;
    char valueBuf[1024],labelBuf[1024];
    int errOccur = 0;    
    char* arrPath[] = {
        "fz2cld:fz2cld:BAS:UPSA:DCDY",
        "fz2cld:fz2cld:PSCADA:100:YFFG",
    };
    soeSize = sizeof(arrPath) / sizeof(char*);    
    
    soes = calloc(soeSize,sizeof(irtdb_fep_soe*));
    if(!soes) return -1;
    for(i = 0; i < soeSize ; i++)
    {        
        sprintf(valueBuf,"value_%d",i+1);
        sprintf(labelBuf,"timeLabel_%d",i+1);
        soe = irtdb_fes_create_fep_soe(arrPath[i],valueBuf,labelBuf,curTime);
        if(!soe)
        {
            errOccur = 1;
            break;
        }
        soes[i] = soe;
    }
    if(!errOccur)
    {
        iRet = irtdb_fes_multiupload_soe(soes,soeSize); 
        if(0 == iRet)
            printf("irtdb_fes_multiupload_soe ok!\n");
        else
            printf("irtdb_fes_multiupload_soe fail!\n");
    }    
    for(i = 0; i < soeSize ; i++)
    {
        soe = soes[i];
        irtdb_fes_free_fep_soe(soe);        
    }
    free(soes);
    return iRet;
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
    iRet = irtdb_fes_start();
    if(-1 == iRet)
    {
        printf("irtdb_fes_start fail!\n");
        return -1;
    }

    printf("start ok , curTime : %ld \n",curTime);
    
    test_upload_soe(curTime);
    test_multiupload_soe(curTime);

    irtdb_fes_stop();
    printf("ok!\n");
    return 0;
}
