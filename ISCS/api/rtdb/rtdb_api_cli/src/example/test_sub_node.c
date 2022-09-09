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

void test_node_notifier(rtdb_cli_subscriber *subscriber,const char *nodePath, const rtdb_cli_record *record)
{
    printf("test_table_notifier start , subscriber uuid : %s!\n",subscriber->uuid);
    printf("test_node_notifier start!\n");
    printf("nodePath : %s \n",nodePath);
    if(record) show_record(record);
}

void test_exception_notifier(rtdb_cli_subscriber *subscriber,int code)
{
    printf("test_exception_notifier uuid : %s, code : %d\n",subscriber->uuid,code);
}

void test_unique_sub_node(rtdb_cli_subscriber *subscriber)
{       
    int code;
    int domainId = 1;
    const char *nodePath = "fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT";
    rtdb_cli_list *attrNameList = NULL;
  
    attrNameList = rtdb_cli_list_create(NULL,NULL);
    if(!attrNameList) return ;
    rtdb_cli_list_push_tail(attrNameList,"di_value");
    rtdb_cli_list_push_tail(attrNameList,"di_status");
    
    code = rtdb_cli_unique_subscribe_node(subscriber,nodePath,attrNameList);  
    if(code == 0)
        printf("test_unique_sub_node : sub ok\n");
    else
    {
        printf("test_unique_sub_node : sub fail\n");
    } 
    rtdb_cli_list_free(attrNameList);    
}

void test_multi_sub_nodes(rtdb_cli_subscriber *subscriber)
{       
    int code;
    int domainId = 1;
    rtdb_cli_list *pathList = NULL;
    rtdb_cli_list *attrNameList = NULL;
  
    pathList = rtdb_cli_list_create(NULL,NULL);
    if(!pathList) return ;
    rtdb_cli_list_push_tail(pathList,"fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT");
    rtdb_cli_list_push_tail(pathList,"fz2cld:fz2cld:BAS:MD-Q1-201:GZZT");
  
    attrNameList = rtdb_cli_list_create(NULL,NULL);
    if(!attrNameList)
    {
        rtdb_cli_list_free(pathList);    
        return ;
    }
    rtdb_cli_list_push_tail(attrNameList,"di_value");
    rtdb_cli_list_push_tail(attrNameList,"di_status");
    
    code = rtdb_cli_multi_subscribe_nodes(subscriber,pathList,attrNameList);  
    if(code == 0)
        printf("rtdb_cli_multi_subscribe_nodes : sub ok\n");
    else
    {
        printf("rtdb_cli_multi_subscribe_nodes : sub fail\n");
    }  
    rtdb_cli_list_free(pathList);      
    rtdb_cli_list_free(attrNameList);      
}

void test_multi_unsub_nodes(rtdb_cli_subscriber *subscriber)
{       
    int code;
    int domainId = 1;
    rtdb_cli_list *pathList = NULL;
  
    pathList = rtdb_cli_list_create(NULL,NULL);
    if(!pathList) return ;
    rtdb_cli_list_push_tail(pathList,"fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT");
    rtdb_cli_list_push_tail(pathList,"fz2cld:fz2cld:BAS:MD-Q1-201:GZZT");
  
   
    code = rtdb_cli_multi_unsubscribe_nodes(subscriber,pathList);  
    if(code == 0)
        printf("rtdb_cli_multi_unsubscribe_nodes : unsub ok\n");
    else
    {
        printf("rtdb_cli_multi_unsubscribe_nodes : unsub fail\n");
    }  
    rtdb_cli_list_free(pathList);      
}

int main()
{
    rtdb_cli_subscriber *subscriber = NULL;
   
    subscriber = rtdb_cli_create_subscriber(
        "nodeSub",test_node_notifier,NULL,test_exception_notifier);
    if(!subscriber) 
    {
        printf("create subscriber fail!\n");
        return -1;
    } 
    test_unique_sub_node(subscriber);
    test_multi_sub_nodes(subscriber);
    rtdb_cli_publish(subscriber);
    sleep(3);
    test_multi_unsub_nodes(subscriber);
    
    // do wait
    while(1)
    {
        sleep(1);
    }    
    rtdb_cli_free_subscriber(subscriber);
    return 0;
}
