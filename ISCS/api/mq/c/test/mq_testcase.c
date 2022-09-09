#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "pthread.h"

#include "mq.h"
#include "mq_priv.h"
typedef  struct testcase_function_entry testcase_function_entry;
typedef int (*testcase_function)(testcase_function_entry* buffer);

int mq_queue_topic_subscriber(void* buffer, const int size, void* replybuff, const int replysize);

int start_test_subscriber(testcase_function_entry* param);
int start_test_sender(testcase_function_entry* param);
int start_test_receiver(testcase_function_entry* param);
int start_stop_test_subscriber(testcase_function_entry* param);
int start_stop_mp_api(testcase_function_entry* param);
int start_send_with_priority(testcase_function_entry* param);
int start_queue_receive_all(testcase_function_entry* param);
int init_mq_api(testcase_function_entry* param);

struct testcase_function_entry{
	int index;
	char* functionname;
	testcase_function func;
	char* topic_queue_name;
	char* payload;
	int ipriority;
};


static testcase_function_entry array_testcaes[] = {
	{0, "start test subscriber", start_test_subscriber, "", "", 0},
	{1, "start test sender",start_test_sender, "", "", 0},
	{2, "start test receiver", start_test_receiver, "", "", 0},
	{3, "start/stop test subscriber",start_stop_test_subscriber, "", "", 0},
	{4, "start/stop mq api",start_stop_mp_api, "", "", 0},
	{5, "queue send with priority", start_send_with_priority, "", "", 0},
	{6, "receive all queue message", start_queue_receive_all, "", "", 0},
	{7, "init mq api", init_mq_api, "", "", 0},
};

/* parameters list
 *  param1: command index
 *  param2: topic / queue name

 
  */
int main(int argc, char **argv){
	if(argc < 2) return;
	char* tqname = "";
	char* payload = "";
	int  ipriority = 0;
	int index = atoi(argv[1]);
	if(argc >2) tqname = argv[2]; 
	if(argc >3) payload = argv[3]; 
	if(argc >4) ipriority = atoi(argv[4]); 
	ipriority = ipriority % 10;
	
	for(int i = 0; i < sizeof(array_testcaes)/sizeof(array_testcaes[0]); i ++) {
		testcase_function_entry item = array_testcaes[i];
		if(item.index == index){
			item.topic_queue_name = tqname;
			item.payload = payload;
			item.ipriority = ipriority;
			item.func(&item);
			break;
		}
	}
}


int start_test_subscriber(testcase_function_entry* param){
	printf("\n\n*** start %s.\n", param->functionname);
	mq_init(NULL);
	printf("\n\n***  mq_subscribe for topic/queue: %s.\n", param->topic_queue_name);
	mq_subscribe("", param->topic_queue_name, mq_queue_topic_subscriber);

	printf("\n****** Hit Enter to exit ******\n");
	getchar();
	printf("*** finish %s.\n", param->functionname);
	mq_release();
}

int start_stop_test_subscriber(testcase_function_entry* param){
	printf("\n\n*** start %s.\n", param->functionname);
	mq_init(NULL);
	printf("\n\n***  mq_subscribe for topic/queue: %s.\n", param->topic_queue_name);
	mq_subscribe("", param->topic_queue_name, mq_queue_topic_subscriber);
	printf("\n****** Hit Enter to unsubscribe ******\n");
	getchar();
	printf("\n\n***  mq_unsubscribe for topic/queue: %s.\n", param->topic_queue_name);
	mq_unsubscribe("", param->topic_queue_name, mq_queue_topic_subscriber);
	
	printf("\n****** Hit Enter to exit ******\n");
	getchar();
	printf("*** finish %s.\n", param->functionname);
	mq_release();
}
int start_test_sender(testcase_function_entry* param){
	printf("\n\n*** start %s.\n", param->functionname);
	mq_init(NULL);
	printf("\n\n*** mq_init done.\n");
	
	if(strlen(param->payload) > 0) {
		int ires = mq_send("", param->topic_queue_name, param->payload, strlen(param->payload), 6);
		printf("*** mq_send res: %d,  name: %s, message: %s.\n", ires, param->topic_queue_name, param->payload);	
	}
	else{
		printf("No valid payload to send!\n");
	}
	printf("\n****** Hit Enter to exit ******\n");
	getchar();
	printf("*** finish %s.\n", param->functionname);
	mq_release();
}

int start_test_receiver(testcase_function_entry* param) {
	char* databuff = NULL;
	int isize = 0;
	char* replybuff = NULL;
	int ireplysize = 0;		

	printf("\n\n*** start %s.\n", param->functionname);
	mq_init(NULL);
	printf("\n\n*** mq_init done.\n");
	
	printf("*** Calling mq_receive for queue: %s\n", param->topic_queue_name);
	while(isize <=0) {
        int ires = mq_receive("", param->topic_queue_name, 6, &databuff, &isize, &replybuff, &ireplysize);
	}
	printf("recv: %s\n", (char*)databuff);

	if(databuff) mq_received_free(databuff);
	if(replybuff) mq_received_free(replybuff);

	printf("\n****** Hit Enter to exit ******\n");
	getchar();
	printf("*** finish %s.\n", param->functionname);
	mq_release();
}
int mq_queue_topic_subscriber(void* buffer, const int size, void* replybuff, const int replysize){
	if(size > 0)
    {
        printf("recv:  %s\n", (char*)buffer);
	}       
}

int start_stop_mp_api(testcase_function_entry* param){
	printf("\n\n*** start %s.\n", param->functionname);
	mq_init(NULL);
	printf("\n\n*** mq_init done.\n");
	
	if(strlen(param->payload) > 0) {
		int ires = mq_send("", param->topic_queue_name, param->payload, strlen(param->payload), 6);
		printf("*** mq_send res: %d,  name: %s, message: %s.\n", ires, param->topic_queue_name, param->payload);	
		
		printf("\n****** Hit Enter to call mq_release ******\n");
		getchar();
		printf("*** Calling mq_release.\n");
		mq_release();	
		
		printf("\n****** Hit Enter to call mq_send again ******\n");
		getchar();
		
		ires = mq_send("", param->topic_queue_name, param->payload, strlen(param->payload), 6);
		printf("*** mq_send res: %d,  name: %s, message: %s.\n", ires, param->topic_queue_name, param->payload);	
		
		printf("\n****** Hit Enter to exit ******\n");
		getchar();
		printf("*** finish %s.\n", param->functionname);
	}
	else{
		printf("No valid payload to send!\n");
	}
}

int start_send_with_priority(testcase_function_entry* param){
	printf("\n\n*** start %s.\n", param->functionname);
	mq_init(NULL);
	printf("\n\n*** mq_init done.\n");
	
	if(strlen(param->payload) > 0) {
		char replyqueue [200];
		snprintf(replyqueue, 200, "ack_%s",param->topic_queue_name );
		MQChannel* queueChnl = mq_channel_create("", param->topic_queue_name, 1, MQ_MSG_REPLY_NO, replyqueue);
		queueChnl->priority = param->ipriority;

		int ires = mq_policy_send(queueChnl, param->payload, strlen(param->payload), 5, param->ipriority);
		printf("*** mq_policy_send res: %d,  name: %s, priority: %d, message: %s.\n", ires, param->topic_queue_name, param->ipriority, param->payload);	
	}
	else{
		printf("No valid payload to send!\n");
	}
	printf("\n****** Hit Enter to exit ******\n");
	getchar();
	printf("*** finish %s.\n", param->functionname);
	mq_release();	
}

int start_queue_receive_all(testcase_function_entry* param){
	char* databuff = NULL;
	int isize = 0;
	char* replybuff = NULL;
	int ireplysize = 0;		

	printf("\n\n*** start %s.\n", param->functionname);
	mq_init(NULL);
	printf("\n\n*** mq_init done.\n");
	
	printf("*** Calling mq_receive for queue: %s\n", param->topic_queue_name);
	do{
        int ires = mq_receive("", param->topic_queue_name, 6, &databuff, &isize, &replybuff, &ireplysize);
		if(isize > 0){
			printf("recv: %s\n", (char*)databuff);
			if(databuff) mq_received_free(databuff);
			if(replybuff) mq_received_free(replybuff);
		}
	}while(isize>0);

	printf("\n****** Hit Enter to exit ******\n");
	getchar();
	printf("*** finish %s.\n", param->functionname);
	mq_release();	
}

int init_mq_api(testcase_function_entry* param){
	printf("\n\n*** start %s.\n", param->functionname);
	mq_init(NULL);
	printf("\n\n*** mq_init done.\n");
	
	printf("\n****** Hit Enter to exit ******\n");
	getchar();
	printf("*** finish %s.\n", param->functionname);
	mq_release();
}