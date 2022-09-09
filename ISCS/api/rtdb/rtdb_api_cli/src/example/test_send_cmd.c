#include "rtdb_client.h"
#include "rtdb_cli_data.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void test_cmdResult_notifier(const void *userData, int domainId, int type, const char* destination, int resultCode, const char *result)
{
    printf("test_cmdResult_notifier called!\n");
    if(!userData)
        printf("userData is NULL!\n");
    else 
        printf("userData is not NULL!\n");
    printf("time : %d\n",time(NULL));
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

void test_send_cmd_fes_1(rtdb_cli* client)
{
    int domainId=1,type=1; // 1 : rtdb ; 2 : plugin ; 3 : fes
    const char *destination = NULL;
    const char *cmdJson = "{"
        "\"user_group_name\": \"ugroup_01\","
        "\"user_name\": \"uname_01\","
        "\"host_name\": \"hname_01\","
        "\"type\": 1,"
        " \"point_label\":\"fz2cld:fz2cld:PSCADA:100:YFFG\", "
        " \"expected_value\":10, "
        " \"opt_time\":1539313741000 "
        "}";
        
    const void *userData = NULL;
 // 需要在 dev_tbl 中设置 rtu_label ， rtdb 会根据 rtu_label 向fes转发数据
    rtdb_cli_send_command(
        client,domainId,type,destination,cmdJson,
        &test_cmdResult_notifier,userData);
    
}

void test_send_cmd_fes_2(rtdb_cli* client)
{
    int domainId=1,type=1; // 1 : rtdb ; 2 : plugin ; 3 : fes
    //const char *destination = "fes";
    const char *destination = NULL;
    const char *cmdJson = "{"
        "\"user_group_name\": \"ugroup_01\","
        "\"user_name\": \"uname_01\","
        "\"host_name\": \"hname_01\","
        "\"type\": 6,"
        " \"point_label\":\"fz2cld:fz2cld:PSCADA:100:YFFG\", "
        " \"expected_value\":12, "
        " \"opt_time\":1539313741000 "
        "}";
        
    const void *userData = NULL;
 // 需要在 dev_tbl 中设置 rtu_label ， rtdb 会根据 rtu_label 向fes转发数据
    rtdb_cli_send_command(
        client,domainId,type,destination,cmdJson,
        &test_cmdResult_notifier,userData);
    
}

void test_send_cmd_fes_3(rtdb_cli* client)
{
    int domainId=1,type=1; // 1 : rtdb ; 2 : plugin ; 3 : fes
    //const char *destination = "fes";
    const char *destination = NULL;
    const char *cmdJson = "{"
        "\"user_group_name\": \"ugroup_01\","
        "\"user_name\": \"uname_01\","
        "\"host_name\": \"hname_01\","
        "\"type\": 10,"
        " \"point_label\":\"dlabel_01:slabel_01:proLabel_01:devLabel_01:aio_label_01\", "
        " \"expected_value\":123.56, "
        " \"opt_time\":1539313741000 "
        "}";
        
    const void *userData = NULL;
 // 需要在 dev_tbl 中设置 rtu_label ， rtdb 会根据 rtu_label 向fes转发数据
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
    test_send_cmd_fes_1(client);
    test_send_cmd_fes_2(client);
    test_send_cmd_fes_3(client);
    
    // do wait
    while(1)
    {
        sleep(1);
    }    
    rtdb_cli_free(client); 
    return 0;
}
