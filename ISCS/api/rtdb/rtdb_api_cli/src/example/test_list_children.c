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

void test_list_children(rtdb_cli* client)
{
    rtdb_cli_list *resultList;
    char *pKey = NULL;
    
    resultList = rtdb_cli_list_children(client,"fz2cld");
    if (resultList)
    {
        pKey = rtdb_cli_list_get_first(resultList);
        while(pKey)
        {
            printf("%s\n",pKey);
            pKey = rtdb_cli_list_get_next(resultList);
        }
        rtdb_cli_list_free(resultList);
    }
    else
        show_last_error(client);
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
    test_list_children(client);
    rtdb_cli_free(client); 
    return 0;
}
