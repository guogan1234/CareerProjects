#include "irtdb_fes_data.h"
#include "irtdb_fes_interface.h"
#include <time.h>
#include <stdio.h>

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
    iRet = irtdb_fes_timed_upload_di(
        "fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT",
        "di_value",663456,curTime
    );
    if(0 == iRet)
        printf("irtdb_fes_timed_upload_di ok!\n");
    else
        printf("irtdb_fes_timed_upload_di fail!\n");
    
    iRet = irtdb_fes_timed_upload_ai(
        "fz2cld:fz2cld:BAS:UPSA:WD",
        123.1,curTime
    );
    if(0 == iRet)
        printf("irtdb_fes_timed_upload_ai ok!\n");
    else
        printf("irtdb_fes_timed_upload_ai fail!\n");
   
    iRet = irtdb_fes_timed_upload_acc(
        "fz2cld:fz2cld:BAS:UPSA:ACC_01",
        654321,curTime
    );
    if(0 == iRet)
        printf("irtdb_fes_timed_upload_acc ok!\n");
    else
        printf("irtdb_fes_timed_upload_acc fail!\n");
  
    irtdb_fes_stop();
	irtdb_fes_uninit();
    printf("ok!\n");
    return 0;
}
