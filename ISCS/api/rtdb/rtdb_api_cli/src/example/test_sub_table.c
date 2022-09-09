#include "rtdb_client.h"
#include "rtdb_cli_data.h"
#include "rtdb_cli_data_public.h"
#include <stdlib.h>
#include <stdio.h>

void show_variant(const rtdb_cli_variant *value)
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
            printf("%lld",value->v.integer);
            break;
        default:
            break;        
    }
    printf("\n");
}

void show_record(const rtdb_cli_record *record)
{
    int i;
    const char *pKey = NULL; 
    const rtdb_cli_variant *value;
    
    for(i = 0; i < rtdb_cli_record_getsize(record); i++)
    {
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

void show_table(const rtdb_cli_list *resultList)
{
    if (resultList)
    {
        rtdb_cli_record *pRecord = NULL;
        pRecord = (rtdb_cli_record *)rtdb_cli_list_get_first((rtdb_cli_list*)resultList);
        while(pRecord)
        {
            show_record(pRecord);
            pRecord = (rtdb_cli_record *)rtdb_cli_list_get_next((rtdb_cli_list*)resultList);
        }
    }
    else
    {
        printf("resultList is NULL!\n");
    }
}

void test_table_notifier(rtdb_cli_subscriber *subscriber,const char *tableName, int type, const rtdb_cli_list *records)
{
    printf("test_table_notifier start , subscriber uuid : %s!\n",subscriber->uuid);
    printf("tableName : %s  type : %d\n",tableName,type);
    show_table(records);
}

void test_exception_notifier(rtdb_cli_subscriber *subscriber,int code)
{
    printf("test_exception_notifier uuid : %s, code : %d\n",subscriber->uuid,code);
}

void test_unique_sub_table(rtdb_cli_subscriber *subscriber)
{       
    int code;
    int domainId = 1;
    const char *tableName = "domain_tbl";    
    
    code = rtdb_cli_unique_subscribe_table(subscriber,domainId,tableName);    
    if(code == 0)
        printf("test_unique_sub_table : sub ok\n");
    else
    {
        printf("test_unique_sub_table : sub fail\n");
    }    
}

void test_multi_sub_tables(rtdb_cli_subscriber *subscriber)
{       
    int code;
    int domainId = 1;    
    rtdb_cli_list *tableNames = NULL;
  
    tableNames = rtdb_cli_list_create(NULL,NULL);
    if(!tableNames) return ;
    rtdb_cli_list_push_tail(tableNames,"domain_tbl");
    rtdb_cli_list_push_tail(tableNames,"unit_tbl");
    rtdb_cli_list_push_tail(tableNames,"station_tbl");
    
    code = rtdb_cli_multi_subscribe_tables(subscriber,domainId,tableNames);
    if(code == 0)
        printf("test_multi_sub_tables : sub ok\n");
    else
    {
        printf("test_multi_sub_tables : sub fail\n");
    }
    rtdb_cli_list_free(tableNames);
}

void test_multi_unsub_tables(rtdb_cli_subscriber *subscriber)
{       
    int code;
    int domainId = 1;    
    rtdb_cli_list *tableNames = NULL;
  
    tableNames = rtdb_cli_list_create(NULL,NULL);
    if(!tableNames) return ;
    rtdb_cli_list_push_tail(tableNames,"domain_tbl2");
    rtdb_cli_list_push_tail(tableNames,"station_tbl");
    
    code = rtdb_cli_multi_unsubscribe_tables(subscriber,domainId,tableNames);
    if(code == 0)
        printf("test_multi_unsub_tables : unsub ok\n");
    else
    {
        printf("test_multi_unsub_tables : unsub fail\n");
    }
    rtdb_cli_list_free(tableNames);
}

int main()
{
    rtdb_cli_subscriber *subscriber = NULL;
   
    subscriber = rtdb_cli_create_subscriber(
        "tblSub",NULL,test_table_notifier,test_exception_notifier);
    if(!subscriber) 
    {
        printf("create subscriber fail!\n");
        return -1;
    } 
    test_unique_sub_table(subscriber);
    test_multi_sub_tables(subscriber);
    rtdb_cli_publish(subscriber);
    sleep(3);
    test_multi_unsub_tables(subscriber);

    // do wait
    while(1)
    {
        sleep(1);
    }    
    rtdb_cli_free_subscriber(subscriber);
    return 0;
}
