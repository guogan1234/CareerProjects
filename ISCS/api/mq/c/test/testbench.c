
// test/test.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "pthread.h"

#include "mq.h"


test_queue_policy_send(int num, char* queuename, char* ackqueuename, int  isAutoAck, int isPrintOn){
    MQChannel* queueChnl = mq_channel_create("", queuename, 0, MQ_MSG_REPLY_MANUAL, ackqueuename);
    printf("mq_channel_create done...\n");

	time_t st_time = 0;
	st_time = time(&st_time);
    for(int i = 0; i< num; i++){
        char databuff[200];
		if(isAutoAck > 0){
			snprintf(databuff, 200, "#%06d: %s", i, "AUTO_ACK message.");
			
			queueChnl->replyMode = MQ_MSG_REPLY_AUTO;
			int iackres = mq_policy_send(queueChnl, databuff, strlen(databuff), 5, 0);
			
			if(isPrintOn > 0) printf("Queue Message (%s)sent, Result: %x\n", databuff, iackres);
		}
		else{
			snprintf(databuff, 200, "#%06d: %s", i, "MANUAL_ACK message.");
			
			queueChnl->replyMode = MQ_MSG_REPLY_MANUAL;
			int iackres = mq_policy_send(queueChnl, databuff, strlen(databuff), 5, 0);
			
			if(isPrintOn > 0) printf("Queue Message (%s) sent, Result: %x\n", databuff, iackres);
		}
		
		if(i%1000 == 999){
			time_t ed_time;
			ed_time = time(&ed_time);
			printf("Processed (%d) with time (%d) seconds. \n", i, (ed_time - st_time));
		}
    }
	
	printf("\n\n++++++++++test_queue_policy_send: Total Sent: %d++++++++++\n\n", num);
}

int g_queue_rece_index = 0;
int g_isprint_on = 1;
int test_queue_subscribe_with_acknowledge_callback(void* buffer, const int size, void* replybuff, const int replysize){
    if(size > 0)
    {
        if(g_isprint_on > 0) printf("Q_%06d: Received: %s\n", g_queue_rece_index, (char*)buffer);
        g_queue_rece_index ++;
		
        if(replysize > 0)
        {
            static int istatuscode = 0;
            if(strstr(buffer, "MANUAL_ACK") != NULL){
                istatuscode = (istatuscode == 0 ? 1 : 0);
            }
            mq_acknowledge(replybuff, replysize, istatuscode, MQ_MSG_TIMEOUT_DEFAULT/1000);
        }
    }
    else
    {
        printf("no data \n");
    }
}

test_queue_subscribe_with_acknowledge(char* queuename, int  isAutoAck, int isPrintOn){
    mq_subscribe("", queuename, test_queue_subscribe_with_acknowledge_callback);
	g_isprint_on = isPrintOn;
}

test_topic_send(int num, char* topicname, int isPrintOn){
	time_t st_time = 0;
	st_time = time(&st_time);
    for(int i = 0; i< num; i++){
        char databuff[200];
        snprintf(databuff, 200, "#%06d: %s", i, "Topic message.");

        int status = mq_send("", topicname, databuff, strlen(databuff), 5);
        if(isPrintOn>0) printf("Topic Message(%s) sent, Result: %x\n", databuff, status);
		
		if(i%1000 == 999){
			time_t ed_time;
			ed_time = time(&ed_time);
			printf("Processed (%d) with time (%d) seconds. \n", i, (ed_time - st_time));
		}
    }
}

int g_topic_rece_index = 0;
time_t  g_topic_st_time=0;
time_t g_topic_ed_time = 0;
int test_topic_subscribe_callback(void* buffer, const int size, void* replybuff, const int replysize){
    if(size > 0)
    {
        if(g_isprint_on > 0) printf("T_%06d: Received: %s\n", g_topic_rece_index, (char*)buffer);
		g_topic_rece_index++;
    }
    else
    {
        printf("no data \n");
    }

	if(g_topic_rece_index == 1){
		g_topic_st_time = time(&g_topic_st_time);
		printf("start time is set: %d\n", g_topic_st_time);
	}
	if(g_topic_rece_index%1000 == 0){
			g_topic_ed_time = time(&g_topic_ed_time);
			//printf("Processed (%d) with time (%d) seconds. \n", g_topic_rece_index, (g_topic_ed_time - g_topic_st_time));
			printf("QPS for topic subscribe is: %d/%d = %d: \n", g_topic_rece_index,   (g_topic_ed_time - g_topic_st_time), g_topic_rece_index / (  (g_topic_ed_time - g_topic_st_time) == 0 ? 1 :  (g_topic_ed_time - g_topic_st_time)) );
	}
}

test_topic_subscribe(char* topic, int isPrintOn){
	g_isprint_on = isPrintOn;
	g_topic_rece_index = 0;
	g_topic_st_time = time(&g_topic_st_time);
    mq_subscribe("", topic, test_topic_subscribe_callback);
}

int main(int argc, char **argv)
{
    mq_init(NULL);
    printf("mq_init done\n");

    if(argc > 1){
		time_t st_time = time(&st_time);

	    char* policyquque = "queue://domain:test_queue_benchmark";
		char* ack_policyquque = "queue://domain:ack_test_queue_benchmark";
		char* topicname = "topic://domain:test_topic_benchmark";
		g_queue_rece_index = 0;
		g_topic_rece_index = 0;
		
		int num=10000;
		if(argc > 2) num = atoi(argv[2]);
		
		int isautoack = 1;
		if(argc > 3) isautoack = atoi(argv[3]);
		
		int isprinton = 1;
		if(argc > 4) isprinton = atoi(argv[4]);
		g_isprint_on = isprinton;
		
        if(strcmp(argv[1], "qpsnd") ==0){
			test_queue_policy_send(num, policyquque, ack_policyquque, isautoack, isprinton);
        }
		else if(strcmp(argv[1], "tsnd") ==0){
            test_topic_send(num, topicname, isprinton);
        }
        else if(strcmp(argv[1], "qsub") == 0){
            test_queue_subscribe_with_acknowledge(policyquque, isautoack, isprinton);
			mq_process_loop();
        }
		else if(strcmp(argv[1], "tsub") == 0){
            test_topic_subscribe(topicname, isprinton);
			mq_process_loop();
        }
//		else if(strcmp(argv[1], "*snd") ==0){
//            test_queue_policy_send(num, policyquque, ack_policyquque, isautoack, isprinton);
//			test_topic_send(num, topicname);
//        }
//		else if(strcmp(argv[1], "*sub") == 0){
//			test_queue_subscribe_with_acknowledge(policyquque, isautoack, isprinton);
//            test_topic_subscribe(topicname);
//			mq_process_loop();
//        }
		time_t ed_time = time(&ed_time);
		printf("QPS for %s is: %d/%d = %d: \n", argv[1], num,  (ed_time - st_time), num / ( (ed_time - st_time) == 0 ? 1 : (ed_time - st_time) ) );
    }

    mq_release();
	getchar();
    return 0;
}



