#include "rtdb_client.h"
#include "rtdb_cli_data.h"
#include <stdlib.h>
#include <stdio.h>

void test_cmdResult_notifier(const void *userData, int domainId, int type, const char* destination, int resultCode, const char *result)
{
    printf("test_cmdResult_notifier called!\n");
    if(!userData)
        printf("userData is NULL!\n");
    else 
        printf("userData is not NULL!\n");
    printf("domainId : %d\n",domainId);
    printf("type : %d\n",type);
    if(destination)
        printf("destination : %s\n",destination);
    else 
        printf("destination is NULL!\n");
    printf("resultCode : %d\n",resultCode);
    if(result)
        printf("result : %s\n",result);
    else
        printf("result is NULL!\n");
}
void test_send_cmd(rtdb_cli* client)
{
    int domainId=1,type=1;
    const char *destination = "fes";
    const char *cmdJson = "{}";
    const void *userData = NULL;
 
    rtdb_cli_send_command(
        client,domainId,type,destination,cmdJson,
        &test_cmdResult_notifier,userData);
    
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
    test_send_cmd(client);
    
    // do wait
    while(1)
    {
        sleep(1);
    }    
    rtdb_cli_free(client); 
    return 0;
}
