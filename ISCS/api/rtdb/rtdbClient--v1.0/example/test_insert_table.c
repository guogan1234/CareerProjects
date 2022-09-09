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

void test_insert_table(rtdb_cli* client,int domain_id)
{
    int code;
    rtdb_cli_record *pRecord = NULL;
    rtdb_cli_variant *value;

    pRecord = rtdb_cli_record_create(4);
    if (!pRecord) return ;
    
    // domain_name
    value = malloc(sizeof(rtdb_cli_variant));
    value->type = RTDB_CLI_VARIANT_STRING;
    value->v.string = strdup("dNameTest1");
    rtdb_cli_record_setvalue(pRecord,"domain_name",value);    
    
    // domain_label
    value = malloc(sizeof(rtdb_cli_variant));
    value->type = RTDB_CLI_VARIANT_STRING;
    value->v.string = strdup("dLableTest1");
    rtdb_cli_record_setvalue(pRecord,"domain_label",value);    
    
    // domain_id
    value = malloc(sizeof(rtdb_cli_variant));
    value->type = RTDB_CLI_VARIANT_INT32;
    value->v.integer = 666;
    rtdb_cli_record_setvalue(pRecord,"domain_id",value);    
    
    // domain_type
    value = malloc(sizeof(rtdb_cli_variant));
    value->type = RTDB_CLI_VARIANT_INT32;
    value->v.integer = 99;
    rtdb_cli_record_setvalue(pRecord,"domain_type",value);
    
    code = rtdb_cli_insert_record(client,domain_id,"domain_tbl","domain_id=666",pRecord);

    if (code == 0)
    {
        printf("rtdb_cli_insert_record ok\n");
    }
    else
    {
        printf("rtdb_cli_insert_record failed\n");
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
    test_insert_table(client,-1);
    rtdb_cli_free(client); 
    return 0;
}
