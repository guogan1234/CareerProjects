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
        // 1ÍøÂç¹ÊÕÏ£¬2·þÎñÆ÷´íÎó£¬3ÆäËü¡£
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

void test_unique_query_device_attribute(rtdb_cli* client)
{
    rtdb_cli_variant *value;  

    if(!client) return;    
    value = rtdb_cli_unique_query_device_attribute(
        client,
        "dlabel_01:slabel_01:proLabel_01:devLabel_01.dev_status"
    );
    if (value)
    {
        show_variant(value);
        rtdb_cli_variant_free(value);
    }
    else
    {
        printf("value is NULL!\n");
        show_last_error(client);
    }
}


void test_unique_query_acc_attribute(rtdb_cli* client)
{
    rtdb_cli_variant *value;
  
    if(!client) return;
    value = rtdb_cli_unique_query_acc_attribute(
        client,
        "dlabel_01:slabel_01:proLabel_01:devLabel_01:acc_label_01.acc_value"
    );
    
    if (value)
    {
        show_variant(value);
        rtdb_cli_variant_free(value);
    }
    else
    {
        printf("value is NULL!\n");
        show_last_error(client);
    }
}

void test_unique_query_dig_attribute(rtdb_cli* client)
{
    rtdb_cli_variant *value;

    if(!client) return;
    value = rtdb_cli_unique_query_dig_attribute(
        client,
        "dlabel_01:slabel_01:proLabel_01:devLabel_01:dio_label_01.di_value"
    );
    if (value)
    {
        show_variant(value);
        rtdb_cli_variant_free(value);
    }
    else
    {
        printf("value is NULL!\n");
        show_last_error(client);
    }    
}

void test_unique_query_ana_attribute(rtdb_cli* client)
{
    rtdb_cli_variant *value;

    if(!client) return;
    value = rtdb_cli_unique_query_ana_attribute(
        client,
        "dlabel_01:slabel_01:proLabel_01:devLabel_01:aio_label_01.ai_value"
    );
    if (value)
    {
        show_variant(value);
        rtdb_cli_variant_free(value);
    }
    else
    {
        printf("value is NULL!\n");
        show_last_error(client);
    }
}

void test_unique_query_attribute(rtdb_cli* client)
{
    rtdb_cli_variant *value;

    if(!client) return;
    value = rtdb_cli_unique_query_attribute(client,"dlabel_01.domain_tbl.domain_name");
    if (value)
    {
        show_variant(value);
        rtdb_cli_variant_free(value);
    }
    else
    {
        printf("value is NULL!\n");
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
    test_unique_query_device_attribute(client);    
    test_unique_query_acc_attribute(client);    
    test_unique_query_dig_attribute(client);    
    test_unique_query_ana_attribute(client);
    test_unique_query_attribute(client);    
    
    rtdb_cli_free(client);    
    return 0;
}
