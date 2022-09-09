#include "irtdb_fes_data.h"
#include "irtdb_fes_interface.h"
#include <time.h>
#include <stdio.h>

int test_timed_multiupload_di(long long curTime)
{
    int iRet = -1;
    irtdb_fep_di **dis = NULL;
    irtdb_fep_di *di = NULL;
    int i = 0,disSize = 0;
    int errOccur = 0;
    char* arrPath[] = {
        "fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT",
        "fz2cld:fz2cld:BAS:MD-Q1-201:GZZT",
        "fz2cld:fz2cld:BAS:MD-Q1-201:SDZDZT",
    };
    disSize = sizeof(arrPath) / sizeof(char*);
    dis = calloc(disSize,sizeof(irtdb_fep_di*));
    if(!dis) return -1;
    for(i = 0; i < disSize; i++)
    { 
        di = irtdb_fes_create_fep_di(arrPath[i],"di_value",1000 + i,curTime);
        if(!di)
        {
            errOccur = 1;
            break;
        }
        dis[i] = di;
    }
    if(!errOccur)
    {
        iRet = irtdb_fes_timed_multiupload_di(dis,disSize); 
        if(0 == iRet)
            printf("irtdb_fes_timed_multiupload_di ok!\n");
        else
            printf("irtdb_fes_timed_multiupload_di fail!\n");
    }    
    for(i = 0; i < disSize ; i++)
    {
        di = dis[i];
        irtdb_fes_free_fep_di(di);        
    }
    free(dis);
    return iRet;
}

int test_timed_multiupload_ai(long long curTime)
{
    int iRet = -1;
    irtdb_fep_ai **ais = NULL;
    irtdb_fep_ai *ai = NULL;
    int i = 0,aisSize = 0;
    char pathBuf[1024];
    int errOccur = 0;
    char* arrPath[] = {
        "fz2cld:fz2cld:BAS:UPSA:WD",
        "fz2cld:fz2cld:BAS:UPSA:SRDY",
        "fz2cld:fz2cld:BAS:UPSA:GZDY",
    };
    aisSize = sizeof(arrPath) / sizeof(char*);
    
    ais = calloc(aisSize,sizeof(irtdb_fep_ai*));
    if(!ais) return -1;
       
    for(i = 0; i < aisSize ; i++)
    {
        ai = irtdb_fes_create_fep_ai(arrPath[i],123.1 + i,curTime);
        if(!ai)
        {
            errOccur = 1;
            break;
        }
        ais[i] = ai;
    }
    if(!errOccur)
    {
        iRet = irtdb_fes_timed_multiupload_ai(ais,aisSize); 
        if(0 == iRet)
            printf("irtdb_fes_timed_multiupload_ai ok!\n");
        else
            printf("irtdb_fes_timed_multiupload_ai fail!\n");
    }    
    for(i = 0; i < aisSize ; i++)
    {
        ai = ais[i];
        irtdb_fes_free_fep_ai(ai);        
    }
    free(ais);
    return iRet;
}

int test_timed_multiupload_acc(long long curTime)
{
    int iRet = -1;
    irtdb_fep_acc **accs = NULL;
    irtdb_fep_acc *acc = NULL;
    int i = 0,accSize = 0;
    char pathBuf[1024];
    int errOccur = 0;    
    char* arrPath[] = {
        "fz2cld:fz2cld:BAS:UPSA:ACC_01",
        "fz2cld:fz2cld:BAS:UPSA:ACC_02",
    };
    accSize = sizeof(arrPath) / sizeof(char*);    
    
    accs = calloc(accSize,sizeof(irtdb_fep_acc*));
    if(!accs) return -1;
    for(i = 0; i < accSize ; i++)
    {
        acc = irtdb_fes_create_fep_acc(arrPath[i],654321 + i,curTime);
        if(!acc)
        {
            errOccur = 1;
            break;
        }
        accs[i] = acc;
    }
    if(!errOccur)
    {
        iRet = irtdb_fes_timed_multiupload_acc(accs,accSize); 
        if(0 == iRet)
            printf("irtdb_fes_timed_multiupload_acc ok!\n");
        else
            printf("irtdb_fes_timed_multiupload_acc fail!\n");
    }    
    for(i = 0; i < accSize ; i++)
    {
        acc = accs[i];
        irtdb_fes_free_fep_acc(acc);        
    }
    free(accs);
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

    printf("start ok , curTime : %ld ms\n",curTime);
    
    test_timed_multiupload_di(curTime);
    test_timed_multiupload_ai(curTime);
    test_timed_multiupload_acc(curTime);
        
    irtdb_fes_stop();
	irtdb_fes_uninit();
    printf("ok!\n");
    return 0;
}
