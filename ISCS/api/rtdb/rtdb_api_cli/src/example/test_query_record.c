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
    printf("\n");
}

void test_query_unique_record(rtdb_cli* client,int domainId)
{
    rtdb_cli_record *pRecord = NULL;
    rtdb_cli_list *filters;
    
    filters = rtdb_cli_list_create(NULL,NULL);
    rtdb_cli_list_push_tail(filters,"domain_id");
    rtdb_cli_list_push_tail(filters,"domain_type");
    rtdb_cli_list_push_tail(filters,"domain_label");
    //rtdb_cli_list_push_tail(filters,"domain_name");
    
    //pRecord = rtdb_cli_unique_query_record(client,domainId,"domain_tbl","domain_id=1",filters);
    pRecord = rtdb_cli_unique_query_record(client,domainId,"domain_tbl","domain_label='tfl'&domain_id=4",filters);
    if(pRecord)
    {
        show_record(pRecord);
        rtdb_cli_record_free(pRecord);
    }
    else
    {
        printf("rtdb_cli_unique_query_record fail\n");
        show_last_error(client);
    }
    
    rtdb_cli_list_free(filters);  
}

void test_query_multi_records(rtdb_cli* client,int domainId)
{
    rtdb_cli_list *resultList;
    rtdb_cli_list *filters;
    
    filters = rtdb_cli_list_create(NULL,NULL);
    rtdb_cli_list_push_tail(filters,"domain_id");
    rtdb_cli_list_push_tail(filters,"domain_type");         
    rtdb_cli_list_push_tail(filters,"domain_label");         
    
    //resultList = rtdb_cli_multi_query_records(client,domainId,"domain_tbl","domain_id=1",filters);
    resultList = rtdb_cli_multi_query_records(client,domainId,"domain_tbl",NULL,filters);
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
        show_last_error(client);
    rtdb_cli_list_free(filters);
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
    test_query_unique_record(client,domainId);
    test_query_multi_records(client,domainId);
    rtdb_cli_free(client); 
    return 0;
}
