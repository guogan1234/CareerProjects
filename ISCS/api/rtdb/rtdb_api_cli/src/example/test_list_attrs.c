#include "rtdb_client.h"
#include "rtdb_cli_data.h"
#include <stdlib.h>
#include <stdio.h>

void test_list_attr_keys(rtdb_cli* client)
{
    rtdb_cli_list *resultList;
    char *pKey = NULL;    
    
    resultList = rtdb_cli_list_attributes(client,"fz2cld.domain_tbl");
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
    test_list_attr_keys(client);
    rtdb_cli_free(client); 
    return 0;
}
