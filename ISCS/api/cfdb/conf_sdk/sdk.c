#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include "thread_param.h"
#include "threadtask.h"
#include "c_list.h"

int conf_module_load_table(const char* table_name,const int domain_id,void (*fp)(const char* table_name,void *record)){
	printf("conf_module_load_table...\n");
    c_list* data = malloc(sizeof(c_list));
	c_list_create(data,NULL);
    //c_list_create(&data,NULL);

    //copy input string
    char* t_name = NULL; 
    if(table_name != NULL){
	int len = strlen(table_name);
	t_name = (char*)malloc(len + 1);
	memset(t_name,'\0',len + 1);
	memcpy(t_name,table_name,len); 
	printf("t_name - %s\n",t_name);
    }   

    //combine thread params
    T_fParam* tParam = (T_fParam*)malloc(sizeof(T_fParam));
    tParam->table_name = t_name;
    //tParam->table_name = table_name;
    tParam->domain_id = domain_id;
    tParam->data = data;
    tParam->t_fp = fp;

//    time_t start;
//    time(&start);

    pthread_t t;
	pthread_create(&t,NULL,t_run,tParam);
	pthread_detach(t);
	printf("after thread detach!\n");
//    pthread_create(&t,NULL,t_run,&tParam);

//    pthread_join(t,NULL);//if no block in main thread,app run finished,with other thread not run.

//    time_t end;
//    time(&end);
//    long s = end - start;
//    printf("SDK:seconds is %ld\n",s);
}
