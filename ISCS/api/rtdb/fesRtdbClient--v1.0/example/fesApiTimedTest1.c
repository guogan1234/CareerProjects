#include "irtdb_fes_data.h"
#include "irtdb_fes_interface.h"
#include <time.h>
#include <stdio.h>

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
    iRet = irtdb_fes_timed_upload_di(
        "dlabel_01:slabel_01:proLabel_01:devLabel_01:dio_label_01",
        "di_value",663456,curTime
    );
    if(0 == iRet)
        printf("irtdb_fes_timed_upload_di ok!\n");
    else
        printf("irtdb_fes_timed_upload_di fail!\n");
    
    iRet = irtdb_fes_timed_upload_ai(
        "dlabel_01:slabel_01:proLabel_01:devLabel_01:aio_label_01",
        123.1,curTime
    );
    if(0 == iRet)
        printf("irtdb_fes_timed_upload_ai ok!\n");
    else
        printf("irtdb_fes_timed_upload_ai fail!\n");
   
    iRet = irtdb_fes_timed_upload_acc(
        "dlabel_01:slabel_01:proLabel_01:devLabel_01:acc_label_01",
        654321,curTime
    );
    if(0 == iRet)
        printf("irtdb_fes_timed_upload_acc ok!\n");
    else
        printf("irtdb_fes_timed_upload_acc fail!\n");
  
    irtdb_fes_stop();
    printf("ok!\n");
    return 0;
}
