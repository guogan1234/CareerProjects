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

void test_update_table(rtdb_cli* client,int domainId)
{
    int code;
    rtdb_cli_record *pRecord = NULL;
    rtdb_cli_variant *value;
       
    pRecord = rtdb_cli_record_create(1);
    if (!pRecord) return ;    
    value = rtdb_cli_variant_create();
    if(!value)
    {
        rtdb_cli_record_free(pRecord);      
        return ; 
    }
    value->type = RTDB_CLI_VARIANT_INT32;
    value->v.integer = 99;    
    
    rtdb_cli_record_setvalue(pRecord,"domain_type",value);
    
    code = rtdb_cli_update_records(client,domainId,"domain_tbl","domain_id=1",pRecord);
    if(code == 0)
    {
        printf("rtdb_cli_update_records ok\n");
    }
    else
    {
        printf("rtdb_cli_update_records fail\n");
        show_last_error(client);        
    }

    rtdb_cli_record_free(pRecord);
}

int main()
{
    rtdb_cli* client = NULL;
    
    client = rtdb_cli_create(NULL);    
    if(!client) 
    {
        printf("create client fail!\n");
        return -1;
    }    
    test_update_table(client,-1);
    rtdb_cli_free(client); 
    return 0;
}
