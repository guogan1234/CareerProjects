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

void test_send_msg(rtdb_cli* client)
{
    int code;
    int domainId = 1;
    int type = 1;
    const char *destination="fes";
    const char *msg = "rtdb_cli_send_message";
    int msgSize = strlen(msg);
    
    code = rtdb_cli_send_message(client,domainId,type,destination,msg,msgSize);

    if(code == 0)
        printf("send ok\n");
    else
    {
        printf("send fail\n");
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
    test_send_msg(client);
    rtdb_cli_free(client); 
    return 0;
}
