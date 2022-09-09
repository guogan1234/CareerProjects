#include "rtdb_client.h"
#include "rtdb_cli_data.h"
#include <stdlib.h>
#include <stdio.h>

void show_variant(rtdb_cli_variant *value)
{    
    switch(value->type)
    {
        case RTDB_CLI_VARIANT_STRING:
            printf("%s",value->v.string);
            break;
        case RTDB_CLI_VARIANT_FLOAT:
        case RTDB_CLI_VARIANT_DOUBLE:
            printf("%lf",value->v.real);
            break;
        case RTDB_CLI_VARIANT_BOOL:
        case RTDB_CLI_VARIANT_INT8:
        case RTDB_CLI_VARIANT_INT16:
        case RTDB_CLI_VARIANT_INT32:
        case RTDB_CLI_VARIANT_INT64:
            printf("%ld",value->v.integer);
            break;
        default:
            break;        
    }
    printf("\n");
}

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

void show_record(rtdb_cli_record *record)
{
    int i;
    rtdb_cli_variant *value;
    for(i = 0; i < rtdb_cli_record_getsize(record); i++)
    {
        char *pKey = NULL; 
        pKey = rtdb_cli_record_getfieldName(record,i);
        if(pKey)
        {            
            printf("%s : \t",pKey);       
        }
        
        value = rtdb_cli_record_getvalue(record,pKey);
        if(value)
        {
            show_variant(value);
        }
    }
    printf("--------------------------------\n");
}

void test_query_table(rtdb_cli* client,int domain_id,const char *tableName,rtdb_cli_list *filters)
{
    rtdb_cli_list *resultList; 
    
    resultList = rtdb_cli_query_table(client,domain_id,tableName,filters);
    if (resultList)
    {
        rtdb_cli_record *pRecord = NULL;
        pRecord = rtdb_cli_list_get_first(resultList);
        while(pRecord)
        {
            show_record(pRecord);
            pRecord = rtdb_cli_list_get_next(resultList);
        }
        rtdb_cli_list_free(resultList);
    }
    else
    {
        printf("resultList is NULL!\n");
        show_last_error(client);
    }
  
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
    
    test_query_table(client,-1,"domain_tbl",NULL);
    
    {
        rtdb_cli_list *filters;
        filters = rtdb_cli_list_create(NULL,NULL);
        rtdb_cli_list_push_tail(filters,"domain_id");
        rtdb_cli_list_push_tail(filters,"domain_type");
          
        test_query_table(client,-1,"domain_tbl",filters);
        rtdb_cli_list_free(filters);    
    }
    rtdb_cli_free(client);
 
    return 0;
}
