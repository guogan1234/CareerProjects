#include "rtdb_client.h"
#include "rtdb_cli_data.h"
#include "rtdb_cli_list.h"
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
    int i,sz;
    rtdb_cli_variant *value;
    sz = rtdb_cli_record_getsize(record);
    if(!sz) 
    {
        printf("record size is zero!\n");
        return ;
    }
    for(i = 0; i < sz; i++)
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
}

void test_query_node(rtdb_cli* client)
{
    rtdb_cli_record *record;

    record = rtdb_cli_query_node(client,"fz2cld");
    if(record)
    {
        show_record(record); 
        rtdb_cli_record_free(record);
    }
    else
    {
        printf("record is NULL!\n");
    }    

    record = rtdb_cli_query_node(client,"fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT");
    if(record)
    {
       show_record(record);
       rtdb_cli_record_free(record);
    }
    else
    {
        printf("record is NULL!\n");
        show_last_error(client);
    }
}

void test_multi_query_nodes(rtdb_cli* client)
{
    rtdb_cli_record *record;
    rtdb_cli_list *paths = NULL;
    rtdb_cli_list *resultList = NULL;
  
    paths = rtdb_cli_list_create(NULL,NULL);
    if(!paths) return ;
    rtdb_cli_list_push_tail(paths,"fz2cld");
    rtdb_cli_list_push_tail(paths,"fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT");

    resultList = rtdb_cli_multi_query_nodes(client,paths);
    if(resultList)
    {
        record = rtdb_cli_list_get_first(resultList);
        while(record)
        {
            show_record(record);
            printf("----------------------\n");
            record = rtdb_cli_list_get_next(resultList);
        }
        rtdb_cli_list_free(resultList);
    }
    else
    {
        printf("record is NULL!\n");
        show_last_error(client);
    }
    rtdb_cli_list_free(paths);
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
    test_query_node(client);    
    test_multi_query_nodes(client);    
    rtdb_cli_free(client);    
    return 0;
}
