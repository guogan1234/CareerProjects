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

void test_get_node_type_base(rtdb_cli* client,const char *spath)
{
    int code;
    int nodeType = 0;
    
    nodeType = rtdb_cli_get_node_type(client,spath); 
    if(nodeType == -1)   
    {
        printf("rtdb_cli_get_node_type fail\n");
        show_last_error(client);
    }
    else
    {
        printf("rtdb_cli_get_node_type ok\n");
        printf("spath : %s , nodeType : %d\n",spath,nodeType);
    }    
}

void test_get_node_type(rtdb_cli* client)
{
    
    test_get_node_type_base(client,"fz2cld:fz2cld:BAS:UPSA:WD");
    test_get_node_type_base(client,"fz2cld:fz2cld:BAS:UPSA");
    test_get_node_type_base(client,"fz2cld:fz2cld:BAS");
    test_get_node_type_base(client,"fz2cld:fz2cld");
    test_get_node_type_base(client,"fz2cld");    
    test_get_node_type_base(client,"fz2cld:fz2cld:BAS:MD-Q1-201:KGWZZT");    
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
    test_get_node_type(client);    
    rtdb_cli_free(client); 
    
    return 0;
}
