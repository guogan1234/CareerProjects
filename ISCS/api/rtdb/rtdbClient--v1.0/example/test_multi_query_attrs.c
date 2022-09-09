#include "rtdb_client.h"
#include "rtdb_cli_data.h"
#include "rtdb_cli_list.h"
#include <stdlib.h>
#include <stdio.h>

typedef rtdb_cli_record* rtdb_cli_multi_query_fun(rtdb_cli* client,const char *path, const rtdb_cli_list *names);


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
            printf("unexpected type");
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

void test_multi_query_attrs_base(rtdb_cli_multi_query_fun query_fun,rtdb_cli* client,const char* path,rtdb_cli_list *nameList)
{
    int i,size;
    rtdb_cli_variant *value;
    rtdb_cli_record *record;
    
    
    record = query_fun(client,path,nameList);
    if(record)
    {
        char *tmpKey = NULL;
        tmpKey = rtdb_cli_list_get_first(nameList);
        while(tmpKey)
        {
            printf("%s : \t",tmpKey);
            value = rtdb_cli_record_getvalue(record,tmpKey);
            show_variant(value);
            tmpKey = rtdb_cli_list_get_next(nameList);
        }        
        rtdb_cli_record_free(record);
    }
    else
    {
        printf("record is NULL!\n");
        show_last_error(client);
    }    
}

void test_multi_query_device_attributes(rtdb_cli* client)
{
    char *path = "dlabel_01:slabel_01:proLabel_01:devLabel_01";
    rtdb_cli_list *nameList = NULL;
  
    nameList = rtdb_cli_list_create(NULL,NULL);
    if(!nameList) return ;
    rtdb_cli_list_push_tail(nameList,"dev_name");
    rtdb_cli_list_push_tail(nameList,"dev_status");
 
    test_multi_query_attrs_base(rtdb_cli_multi_query_device_attributes,client,path,nameList);

    rtdb_cli_list_free(nameList);
}

void test_multi_query_acc_attributes(rtdb_cli* client)
{
    char *path = "dlabel_01:slabel_01:proLabel_01:devLabel_01:acc_label_01";
    rtdb_cli_list *nameList = NULL;
  
    nameList = rtdb_cli_list_create(NULL,NULL);
    if(!nameList) return ;
    rtdb_cli_list_push_tail(nameList,"acc_name");
    rtdb_cli_list_push_tail(nameList,"acc_value");
 
    test_multi_query_attrs_base(&rtdb_cli_multi_query_acc_attributes,client,path,nameList);

    rtdb_cli_list_free(nameList);
}

void test_multi_query_dig_attributes(rtdb_cli* client)
{
    char *path = "dlabel_01:slabel_01:proLabel_01:devLabel_01:dio_label_01";
    rtdb_cli_list *nameList = NULL;
  
    nameList = rtdb_cli_list_create(NULL,NULL);
    if(!nameList) return ;
    rtdb_cli_list_push_tail(nameList,"di_value");
    rtdb_cli_list_push_tail(nameList,"di_status");
 
    test_multi_query_attrs_base(&rtdb_cli_multi_query_dig_attributes,client,path,nameList);

    rtdb_cli_list_free(nameList);
}

void test_multi_query_ana_attributes(rtdb_cli* client)
{
    char *path = "dlabel_01:slabel_01:proLabel_01:devLabel_01:aio_label_01";
    rtdb_cli_list *nameList = NULL;
  
    nameList = rtdb_cli_list_create(NULL,NULL);
    if(!nameList) return ;
    rtdb_cli_list_push_tail(nameList,"ai_value");
    rtdb_cli_list_push_tail(nameList,"ai_status");
 
    test_multi_query_attrs_base(&rtdb_cli_multi_query_ana_attributes,client,path,nameList);

    rtdb_cli_list_free(nameList);
}

void test_multi_query_attributes(rtdb_cli* client)
{
    char *path = "dlabel_01";
    rtdb_cli_list *nameList = NULL;
  
    nameList = rtdb_cli_list_create(NULL,NULL);
    if(!nameList) return ;
    rtdb_cli_list_push_tail(nameList,"domain_tbl.domain_id");
    rtdb_cli_list_push_tail(nameList,"domain_tbl.domain_name");
 
    test_multi_query_attrs_base(&rtdb_cli_multi_query_attributes,client,path,nameList);

    rtdb_cli_list_free(nameList);
}

void test_mmulti_query_attributes(rtdb_cli* client)
{
    rtdb_cli_variant *value;
    rtdb_cli_list *paths = NULL;
    rtdb_cli_list *names = NULL;
    rtdb_cli_list *resultList = NULL;
  
    paths = rtdb_cli_list_create(NULL,NULL);
    if(!paths) return ;
    rtdb_cli_list_push_tail(paths,"dlabel_01:slabel_01:proLabel_01:devLabel_01");
    rtdb_cli_list_push_tail(paths,"dlabel_01:slabel_01:proLabel_01:devLabel_02");

    names = rtdb_cli_list_create(NULL,NULL);
    if(!names) return ;
    rtdb_cli_list_push_tail(names,"dev_name");
    rtdb_cli_list_push_tail(names,"dev_status");
    
    resultList = rtdb_cli_mmulti_query_attributes(client,paths,names);
    if(resultList)
    {
        value = rtdb_cli_list_get_first(resultList);
        while(value)
        {
            show_variant(value);
            value = rtdb_cli_list_get_next(resultList);
        }
        rtdb_cli_list_free(resultList);
    }
    else
    {
        printf("record is NULL!\n");
        show_last_error(client);
    }
    rtdb_cli_list_free(paths);
    rtdb_cli_list_free(names);
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
    
    test_multi_query_device_attributes(client);
    test_multi_query_acc_attributes(client);
    test_multi_query_dig_attributes(client);
    test_multi_query_ana_attributes(client);    
    test_multi_query_attributes(client); 
    test_mmulti_query_attributes(client); 
    
    rtdb_cli_free(client); 
    return 0; 
}
