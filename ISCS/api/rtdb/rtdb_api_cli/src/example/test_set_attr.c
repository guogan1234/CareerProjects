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

void test_set_attr(rtdb_cli* client)
{
    int code;

    code = rtdb_cli_set_attribute(client,"fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT.di_value",
    "123");

    if(code == 0)
        printf("set ok\n");
    else
    {
        printf("set fail\n");
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
    test_set_attr(client);
    rtdb_cli_free(client); 
    return 0;
}
