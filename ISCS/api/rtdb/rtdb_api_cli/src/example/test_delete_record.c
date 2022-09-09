#include "rtdb_client.h"
#include "rtdb_cli_data.h"
#include <stdlib.h>
#include <stdio.h>

void show_last_error(rtdb_cli* client)
{
    int ec = 0;
    
    if(!client) return ;
    ec = rtdb_cli_get_lasterror(client);
    switch(ec)
    {
        // 1网络故障，2服务器错误，3其它。
        case 1:
            printf("network error!\n");
            break;
        case 2:
            printf("server error!\n");
            break;
        case 3:            
            printf("other error!\n");
            break;
        default:
            break;
    }    
}


void test_delete_record(rtdb_cli* client,int domain_id)
{
    int code;
    code = rtdb_cli_delete_records(client,domain_id,"region_tbl",NULL);
    //code = rtdb_cli_delete_records(client,domain_id,"region_tbl","region_id=1");
    if(code == 0)
    {
        printf("rtdb_cli_delete_records ok\n");
    }
    else
    {
        printf("rtdb_cli_delete_records fail\n");
        show_last_error(client);
    }
  
}

int main()
{
    rtdb_cli* client = NULL;
    int domainId = 1;
    
    client = rtdb_cli_create(NULL);    
    if(!client) 
    {
        printf("create client fail!\n");
        return -1;
    }    
    
    test_delete_record(client,domainId);
    
    rtdb_cli_free(client);    
    return 0;
}
