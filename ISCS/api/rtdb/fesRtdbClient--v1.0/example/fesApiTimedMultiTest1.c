#include "irtdb_fes_data.h"
#include "irtdb_fes_interface.h"
#include <time.h>
#include <stdio.h>

int test_timed_multiupload_di(time_t curTime)
{
    int iRet = -1;
    irtdb_fep_di **dis = NULL;
    irtdb_fep_di *di = NULL;
    int i = 0,disSize = 3;
    char pathBuf[1024];
    int errOccur = 0;
    
    dis = calloc(disSize,sizeof(irtdb_fep_di*));
    if(!dis) return -1;
    for(i = 0; i < disSize ; i++)
    {
        sprintf(pathBuf,"dlabel_01:slabel_01:proLabel_01:devLabel_01:dio_label_0%d",i+1);
        
        di = irtdb_fes_create_fep_di(pathBuf,"di_value",1000 + i,curTime);
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

int test_timed_multiupload_ai(time_t curTime)
{
    int iRet = -1;
    irtdb_fep_ai **ais = NULL;
    irtdb_fep_ai *ai = NULL;
    int i = 0,aisSize = 3;
    char pathBuf[1024];
    int errOccur = 0;
    
    ais = calloc(aisSize,sizeof(irtdb_fep_ai*));
    if(!ais) return -1;
    for(i = 0; i < aisSize ; i++)
    {
        sprintf(pathBuf,"dlabel_01:slabel_01:proLabel_01:devLabel_01:aio_label_0%d",i+1);
        
        ai = irtdb_fes_create_fep_ai(pathBuf,123.1 + i,curTime);
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

int test_timed_multiupload_acc(time_t curTime)
{
    int iRet = -1;
    irtdb_fep_acc **accs = NULL;
    irtdb_fep_acc *acc = NULL;
    int i = 0,accSize = 3;
    char pathBuf[1024];
    int errOccur = 0;
    
    accs = calloc(accSize,sizeof(irtdb_fep_acc*));
    if(!accs) return -1;
    for(i = 0; i < accSize ; i++)
    {
        sprintf(pathBuf,"dlabel_01:slabel_01:proLabel_01:devLabel_01:acc_label_0%d",i+1);
        
        acc = irtdb_fes_create_fep_acc(pathBuf,654321 + i,curTime);
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
    time_t curTime = time(NULL);
    char *fesId = "domain_label:station_id:pro_system_label" \
        " rtuid_101   rtuid_102   rtuid_103";
    
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

    printf("start ok , curTime : %d\n",(int)curTime);
    
    test_timed_multiupload_di(curTime);
    test_timed_multiupload_ai(curTime);
    test_timed_multiupload_acc(curTime);
        
    irtdb_fes_stop();
    printf("ok!\n");
    return 0;
}
