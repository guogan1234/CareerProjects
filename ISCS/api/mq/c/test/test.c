
// test/test.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "pthread.h"

#include "mq.h"
#include "mq_priv.h"
#define MQTESTDOMAIN "zydjz"

int messageReceived = 0;
int TopicMessageListener(void* buffer, const int size, void* replybuff, const int replysize){
    printf("TopicMessageListener entry, %s\n", (char*)buffer);
    
    messageReceived ++;
    return 0;
}


int QueueMessageListener(void* buffer, const int size, void* replybuff, const int replysize){
    printf("QueueMessageListener entry, %s\n", (char*)buffer);
    return 0;
}

int QueueMessageSubListenerFromChannel(void* buffer, const int size, void* replybuff, const int replysize){
    if(size > 0)
    {
        printf("Received Message: %s\n", (char*)buffer);
        
        if(replysize > 0)
        {
            int istatuscode = 0;
            if(strstr(buffer, "(NACK") != NULL){
                istatuscode = 1;
            }
            else if(strstr(buffer, "(TIMEOUT)") != NULL){
                istatuscode = 9;
            }
            
            if(istatuscode!=9)
                mq_acknowledge(replybuff, replysize, istatuscode, MQ_MSG_TIMEOUT_DEFAULT/1000);
        }
    }
    else
    {
        printf("no data \n");
    }
}

static int index_response = 0;
static int _flag_stop_QueueReceive_with_Policy_Send = 0;
void *QueueReceive_with_Policy_Send(void *ptr)
{
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    while(_flag_stop_QueueReceive_with_Policy_Send == 0)
    {
        mq_receive(MQTESTDOMAIN, (char*)ptr, 5, &databuff, &isize, &replybuff, &ireplysize);
        //printf("databuff size is %d, isize is %d, replybuff is: (%s).\n", (int)strlen(databuff), isize, (replybuff==NULL ? "" : replybuff));
        if(isize > 0)
        {
            printf("Received Message: %s\n", (char*)databuff);
            
            if(ireplysize > 0)
            {
                index_response ++;
                
                int istatuscode = 0;
                if(strstr(databuff, "(NACK") != NULL){
                    istatuscode = 255;
                }
                else if(strstr(databuff, "(TIMEOUT)") != NULL){
                    istatuscode = 256;
                }
                
                if(istatuscode!=256)
                    mq_acknowledge(replybuff, ireplysize, istatuscode, MQ_MSG_TIMEOUT_DEFAULT/1000);
            }
            mq_received_free(databuff);
            mq_received_free(replybuff);
        }
        else
        {
            printf("no data \n");
        }
    }
    return  ptr;
}

void test_queue_send_receive_mass(){
    char* squeue = "queue://test_queue_send_receive_mass";
    char* queueMsg = "test_queue_send_receive_mass: one to one message";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
	printf("\ntest_queue_send_receive_mass...\n");
    for(int i = 0; i < 100; i ++)
    {
        char databuff[100];
        snprintf(databuff, 100, "#%03d: %s", i, queueMsg);
        mq_send(MQTESTDOMAIN, squeue, databuff, strlen(databuff), MQ_MSG_TIMEOUT_DEFAULT);
        printf("#%03d: Sent Message: %s\n", i, (char*)databuff);
    }
    
    for(int i = 0; i < 100; i ++)
    {
        mq_receive(MQTESTDOMAIN, squeue, 6, &databuff, &isize, &replybuff, &ireplysize);
        if(isize > 0)
        {
            printf("#%03d: Received Message: %s\n", i, (char*)databuff);
        }
        else
        {
            printf("no data \n");
        }
        mq_received_free(databuff);
        mq_received_free(replybuff);
    }
}

void test_message_encode_decode(){
//  Test Mesage Encode/Decode
//    char* topic = "topic://startScripts";
//    char* topicMsg = "broadcast info";
//    
//    char* squeue = "queue://domain:queue_box";
//    char* queueMsg = "one to one message";
//    
//    char* squeue2 = "queue://domain:queue_box2";
//    char* ack_squeue2 = "queue://domain:ack_queue_box2";
//    
//    char* databuff = NULL;
//    int isize = 0;
//    char* replybuff = NULL;
//    int ireplysize = 0;
//    
//    MQChannel chn;
//    memset(&chn, 0, sizeof(MQChannel));
//    strncpy(chn.domain, "localhost", MQ_DOMAIN_SIZE);
//    strncpy(chn.queName, "queue_box", MQ_TOPIC_SIZE);
//    strncpy(chn.replyQueue, "ack_queue_box", MQ_TOPIC_SIZE);
//    
//    MQMessage * oneMsg = mq_msg_create(topic, topicMsg, strlen(topicMsg), NULL);
//    
//    char* pchar = msg_json_serialize(oneMsg);
//    printf("%s\n", pchar);
//    
//    MQMessage* newMsg = msg_json_parse(pchar);
//    printf("%s\n", newMsg->payload);
//    
//    free(pchar);
//    free(newMsg);
//    free(oneMsg);
}

void test_queue_policy_send_receive(){
    char* squeue2 = "queue://test_queue_policy_send_receive";
    char* ack_squeue2 = "queue://ack_test_queue_policy_send_receive";
	
	char* queueMsg = "one to one message";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
    //  Queue Message Policy Send / Receive
    MQChannel* queueChnl = mq_channel_create(MQTESTDOMAIN, squeue2, 1, MQ_MSG_REPLY_MANUAL, ack_squeue2);
    printf("\ntest_queue_policy_send_receive...\n");
    
    pthread_t policy_send_receive_thread;
	_flag_stop_QueueReceive_with_Policy_Send = 0;
    pthread_create(&policy_send_receive_thread, NULL, QueueReceive_with_Policy_Send, (void *) squeue2);

    queueChnl->replyMode = MQ_MSG_REPLY_AUTO;
	for(int i =0; i < 20; i++){
		int iackres = mq_policy_send(queueChnl, "auto reply queue message.", strlen("auto reply queue message."), 5, 1);
		printf("AUTO Queue Message %s is sent with Result: %x\n", queueMsg, iackres);
	}
    queueChnl->replyMode = MQ_MSG_REPLY_MANUAL;
	for(int i=0; i< 10; i++){
		int iackres2 = mq_policy_send(queueChnl, "manual reply queue message (OK).", strlen("manual reply queue message (OK)."), 5, 1);
		printf("MANUAL Queue Message %s. Result: %x\n", "manual reply queue message (OK).", iackres2);
	}
	
    for(int i=0; i< 10; i++){
        int iackres3 = mq_policy_send(queueChnl, "manual reply queue message (NACK1).", strlen("manual reply queue message (NACK1)."), 5, 1);
        printf("#%02d: MANUAL Queue Message %s. Result: %x\n", i, "manual reply queue message (NACK1).", iackres3);
    }
   
    for(int i = 0; i< 2; i++){
        int iackres4 = mq_policy_send(queueChnl, "manual reply queue message (TIMEOUT).", strlen("manual reply queue message (TIMEOUT)."), 5, 1);
        printf("#%02d: MANUAL Queue Message %s is sent with Result: %x\n", i, queueMsg, iackres4);
    }
	_flag_stop_QueueReceive_with_Policy_Send = 1;
    
     mq_channel_destroy(queueChnl);
}

void test_queue_policy_send_receive_withzip(){
    char* squeue2 = "queue://test_zip_queue_policy_send_receive";
    char* ack_squeue2 = "queue://ack_test_zip_queue_policy_send_receive";
	
	char* queueMsg = "INSERT INTO REFEODCOMPONENTTYPE (EODCOMPONENTTYPEID,EODCLASSTYPEID,COMPONENTCODE,MCFCODE,ISVISIBLE,ISADDITIONALFILE,WORDING,ENGWORDING,EQUIPMENTTYPEID,CREATETIME,PROVIDER,EDITABLE,DELETED,XDRCOMPONENTTYPE) VALUES (16433,1,'SystemParam','ACCEOD',1,0,'系统控制参数','',null,null,'',0,0,16433);                         "
"INSERT INTO REFEODCOMPONENTTYPE (EODCOMPONENTTYPEID,EODCLASSTYPEID,COMPONENTCODE,MCFCODE,ISVISIBLE,ISADDITIONALFILE,WORDING,ENGWORDING,EQUIPMENTTYPEID,CREATETIME,PROVIDER,EDITABLE,DELETED,XDRCOMPONENTTYPE) VALUES (16434,1,'NetworkTopoParam','ACCEOD',1,0,'网络拓扑参数','',null,null,'',0,0,16434);                    "
"INSERT INTO REFEODCOMPONENTTYPE (EODCOMPONENTTYPEID,EODCLASSTYPEID,COMPONENTCODE,MCFCODE,ISVISIBLE,ISADDITIONALFILE,WORDING,ENGWORDING,EQUIPMENTTYPEID,CREATETIME,PROVIDER,EDITABLE,DELETED,XDRCOMPONENTTYPE) VALUES (16437,2,'CalendarParam','ACCEOD',1,0,'日历时间参数','',null,null,'',0,0,16437);                       "
"INSERT INTO REFEODCOMPONENTTYPE (EODCOMPONENTTYPEID,EODCLASSTYPEID,COMPONENTCODE,MCFCODE,ISVISIBLE,ISADDITIONALFILE,WORDING,ENGWORDING,EQUIPMENTTYPEID,CREATETIME,PROVIDER,EDITABLE,DELETED,XDRCOMPONENTTYPE) VALUES (16438,2,'FareParam','ACCEOD',1,0,'费率表参数','',null,null,'',0,0,16438);                             "
"INSERT INTO REFEODCOMPONENTTYPE (EODCOMPONENTTYPEID,EODCLASSTYPEID,COMPONENTCODE,MCFCODE,ISVISIBLE,ISADDITIONALFILE,WORDING,ENGWORDING,EQUIPMENTTYPEID,CREATETIME,PROVIDER,EDITABLE,DELETED,XDRCOMPONENTTYPE) VALUES (16435,2,'TicketParam','ACCEOD',1,0,'车票参数','',null,null,'',0,0,16435);                            "
"INSERT INTO REFEODCOMPONENTTYPE (EODCOMPONENTTYPEID,EODCLASSTYPEID,COMPONENTCODE,MCFCODE,ISVISIBLE,ISADDITIONALFILE,WORDING,ENGWORDING,EQUIPMENTTYPEID,CREATETIME,PROVIDER,EDITABLE,DELETED,XDRCOMPONENTTYPE) VALUES (16436,2,'BonusParam','ACCEOD',1,0,'优惠方案参数','',null,null,'',0,0,16436);                          "
"INSERT INTO REFEODCOMPONENTTYPE (EODCOMPONENTTYPEID,EODCLASSTYPEID,COMPONENTCODE,MCFCODE,ISVISIBLE,ISADDITIONALFILE,WORDING,ENGWORDING,EQUIPMENTTYPEID,CREATETIME,PROVIDER,EDITABLE,DELETED,XDRCOMPONENTTYPE) VALUES (16471,8,'GJICAllBlackList','ACCOTH',1,0,'公交IC 卡黑名单','',null,null,'',0,0,16471);                 ";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
    //  Queue Message Policy Send / Receive
    MQChannel* queueChnl = mq_channel_create(MQTESTDOMAIN, squeue2, 1, MQ_MSG_REPLY_MANUAL, ack_squeue2);
	queueChnl->zipMode = 1;
    printf("\ntest_zip_queue_policy_send_receive...\n");
    
    pthread_t policy_send_receive_thread;
	_flag_stop_QueueReceive_with_Policy_Send = 0;
    pthread_create(&policy_send_receive_thread, NULL, QueueReceive_with_Policy_Send, (void *) squeue2);

    queueChnl->replyMode = MQ_MSG_REPLY_AUTO;
	for(int i =0; i < 20; i++){
		int iackres = mq_policy_send(queueChnl, queueMsg, strlen(queueMsg), 5, 1);
		//printf("AUTO Queue Message %s is sent with Result: %x\n", queueMsg, iackres);
	}

	_flag_stop_QueueReceive_with_Policy_Send = 1;
    
     mq_channel_destroy(queueChnl);
}

void test_queue_policy_send_subscribe(){
    char* squeue3 = "queue://test_queue_policy_send_subscribe";
    char* ack_squeue3 = "queue://ack_test_queue_policy_send_subscribe";
    char* queueMsg = "one to one message";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
    //  Queue Message Policy Send / Receive
    MQChannel* queueChnl = mq_channel_create(MQTESTDOMAIN, squeue3, 1, MQ_MSG_REPLY_MANUAL, ack_squeue3);
    printf("\ntest_queue_policy_send_subscribe...\n");
    
    mq_subscribe(MQTESTDOMAIN, squeue3, QueueMessageSubListenerFromChannel);

    queueChnl->replyMode = MQ_MSG_REPLY_AUTO;
    int iackres = mq_policy_send(queueChnl, "auto reply queue message.", strlen("auto reply queue message."), 5, 0);
    printf("AUTO Queue Message %s is sent with Result: %x\n", queueMsg, iackres);

    queueChnl->replyMode = MQ_MSG_REPLY_MANUAL;
    int iackres2 = mq_policy_send(queueChnl, "manual reply queue message (OK).", strlen("manual reply queue message (OK)."), 5, 0);
    printf("MANUAL Queue Message %s is sent with Result: %x\n", queueMsg, iackres2);

    for(int i=0; i< 1000; i++){
        int iackres3 = mq_policy_send(queueChnl, "manual reply queue message (NACK1).", strlen("manual reply queue message (NACK1)."), 5, 0);
        printf("#%02d: MANUAL Queue Message %s is sent with Result: %x\n", i, queueMsg, iackres3);
    }
   
    for(int i = 0; i< 2; i++){
        int iackres4 = mq_policy_send(queueChnl, "manual reply queue message (TIMEOUT).", strlen("manual reply queue message (TIMEOUT)."), 5, 0);
        printf("#%02d: MANUAL Queue Message %s is sent with Result: %x\n", i, queueMsg, iackres4);
    }
    
    mq_channel_destroy(queueChnl);
}


void test_queue_send_receive_stop(){
    char* squeue = "queue://test_queue_send_receive";
    char* queueMsg1 = "#1: test_queue_send_receive: one to one message";
	char* queueMsg2 = "#2: test_queue_send_receive: one to one message";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
    //  Queue Message Send / Receive
    printf("\ntest_queue_send_receive...\n");
	mq_init(NULL);
    mq_send(MQTESTDOMAIN, squeue, queueMsg1, strlen(queueMsg1), MQ_MSG_TIMEOUT_DEFAULT);
	mq_send(MQTESTDOMAIN, squeue, queueMsg2, strlen(queueMsg2), MQ_MSG_TIMEOUT_DEFAULT);
	mq_receive(MQTESTDOMAIN, squeue, 5, &databuff, &isize, & replybuff, &ireplysize);
	if(isize > 0)
	{
		printf("Received Message: %s\n", (char*)databuff);
		mq_received_free(databuff);
		mq_received_free(replybuff);        
	}
	else
	{
		printf("no data \n");
	}
	mq_release();

    printf("\ntest_queue_send_receive ****** reinitialize...\n");
	mq_init(NULL);
	mq_receive(MQTESTDOMAIN, squeue, 5, &databuff, &isize, & replybuff, &ireplysize);
	if(isize > 0)
	{
		printf("Received Message: %s\n", (char*)databuff);
		mq_received_free(databuff);
		mq_received_free(replybuff);        
	}
	else
	{
		printf("no data \n");
	}
	mq_release();
}

void test_queue_send_receive(){
    char* squeue = "queue://test_queue_send_receive";
    char* queueMsg = "test|_queue_send_receive: one to one message";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
    //  Queue Message Send / Receive
    printf("\ntest_queue_send_receive...\n");
    mq_send(MQTESTDOMAIN, squeue, queueMsg, strlen(queueMsg), MQ_MSG_TIMEOUT_DEFAULT);
	mq_receive(MQTESTDOMAIN, squeue, 5, &databuff, &isize, & replybuff, &ireplysize);
	if(isize > 0)
	{
		printf("Received Message: %s\n", (char*)databuff);
		mq_received_free(databuff);
		mq_received_free(replybuff);        
	}
	else
	{
		printf("no data \n");
	}
}

void test_queue_pub_sub(){
    char* queueMsg = "one to one message";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
//  Queue Message Pub / Sub
    printf("\ntest_queue_pub_sub...\n");
    char* squeue = "queue://test_queue_pub_sub";
    mq_subscribe(MQTESTDOMAIN, squeue, QueueMessageListener);
    sleep(1);
    for(int i = 0; i < 200; i ++)
    {
        char buff[200];
        memset(buff, 0, sizeof(buff));
        snprintf(buff, 200, "%s %d", queueMsg, i);
        mq_send(MQTESTDOMAIN, squeue, buff, strlen(buff), MQ_MSG_TIMEOUT_DEFAULT);
    }
}

void test_topic_send_receive(){
    char* topic = "topic://startScripts";
    char* topicMsg = "broadcast info";
    
    char* squeue = "queue://queue_box";
    char* queueMsg = "one to one message";
    
    char* squeue2 = "queue://queue_box2";
    char* ack_squeue2 = "queue://ack_queue_box2";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
//  Topic Message Send / Receive
//    char* databuff = NULL;
//    int isize = 0;
    printf("\nTopic message Send/Receive...\n");
    mq_send(MQTESTDOMAIN, topic, topicMsg, strlen(topicMsg), MQ_MSG_TIMEOUT_DEFAULT);
	//sleep(1);
    for(int i = 0; i < 2; i ++)
    {
        mq_receive(MQTESTDOMAIN, topic, 1, &databuff, &isize, &replybuff, &ireplysize);
        if(isize > 0)
        {
            printf("%s\n", (char*)databuff);
        }
        else
        {
            printf("no data \n");
        }
        mq_received_free(databuff);
        mq_received_free(replybuff);
        
        char buff[200];
        memset(buff, 0, sizeof(buff));
        snprintf(buff, 200, "%s %d", topicMsg, i);
        mq_send(MQTESTDOMAIN, topic, buff, strlen(buff), MQ_MSG_TIMEOUT_DEFAULT);
    }   
}

void test_topic_pub_sub(){
    char* topic = "topic://startScripts";
    char* topicMsg = "broadcast info";
    
    char* squeue = "queue://queue_box";
    char* queueMsg = "one to one message";
    
    char* squeue2 = "queue://queue_box2";
    char* ack_squeue2 = "queue://ack_queue_box2";
    
    char* databuff = NULL;
    int isize = 0;
    char* replybuff = NULL;
    int ireplysize = 0;
    
//  Topic Message Pub / Sub
    printf("\nTopic message Pub/Sub...\n");
    topic="topic://startScripts_pub";
    mq_subscribe(MQTESTDOMAIN, topic, TopicMessageListener);
    mq_send(MQTESTDOMAIN, "topic://startScripts", "hello", strlen("hello"), MQ_MSG_TIMEOUT_DEFAULT);
    for(int i = 0; i < 1000; i ++)
    {
        char buff[200];
        memset(buff, 0, sizeof(buff));
        snprintf(buff, 200, "%s %d", topicMsg, i);
        mq_send(MQTESTDOMAIN, topic, buff, strlen(buff), MQ_MSG_TIMEOUT_DEFAULT);
    }
}

test_queue_policy_send(int num, char* queuename, char* ackqueuename){
    MQChannel* queueChnl = mq_channel_create("", queuename, 0, MQ_MSG_REPLY_MANUAL, ackqueuename);
    printf("mq_channel_create done...\n");

    for(int i = 0; i< num; i++){
        char databuff[200];
		if(num % 2 == 1){
			snprintf(databuff, 200, "#%06d: %s", i, "AUTO_ACK message.");
			queueChnl->replyMode = MQ_MSG_REPLY_AUTO;
			int iackres = mq_policy_send(queueChnl, databuff, strlen(databuff), 5, 0);
			printf("Queue Message (%s)sent, Result: %x\n", databuff, iackres);
		}
		else{
			snprintf(databuff, 200, "#%06d: %s", i, "MANUAL_ACK message.");
			queueChnl->replyMode = MQ_MSG_REPLY_MANUAL;
			int iackres = mq_policy_send(queueChnl, databuff, strlen(databuff), 5, 0);
			printf("Queue Message (%s) sent, Result: %x\n", databuff, iackres);
		}
    }
	
	printf("\n\n++++++++++test_queue_policy_send: Total Sent: %d++++++++++\n\n", num);
}

int g_queue_rece_index = 0;
int test_queue_subscribe_with_acknowledge_callback(void* buffer, const int size, void* replybuff, const int replysize){
    if(size > 0)
    {
        printf("Q_%06d: Received: %s\n", g_queue_rece_index, (char*)buffer);
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

test_queue_subscribe_with_acknowledge(char* queuename){
    mq_subscribe(MQTESTDOMAIN, queuename, test_queue_subscribe_with_acknowledge_callback);
}

test_topic_send(int num, char* topicname){

    for(int i = 0; i< num; i++){
        char databuff[200];
        snprintf(databuff, 200, "#%06d: %s", i, "Topic message.");

        int status = mq_send(MQTESTDOMAIN, topicname, databuff, strlen(databuff), 5);
        printf("Topic Message(%s) sent, Result: %x\n", databuff, status);
    }
}

int g_topic_rece_index = 0;
int test_topic_subscribe_callback(void* buffer, const int size, void* replybuff, const int replysize){
    if(size > 0)
    {
        printf("T_%06d: Received: %s\n", g_topic_rece_index, (char*)buffer);
		g_topic_rece_index++;
    }
    else
    {
        printf("no data \n");
    }
}

test_topic_subscribe(char* topic){
    mq_subscribe(MQTESTDOMAIN, topic, test_topic_subscribe_callback);
}

int main(int argc, char **argv)
{
	//test_queue_send_receive_stop();
	//return;
    mq_init(NULL);
    printf("mq_init done\n");
    if(argc > 1){
	    char* policyquque = "queue://domain:test_queue_policy_send_sub";
		char* ack_policyquque = "queue://domain:ack_test_queue_policy_send_sub";
		char* topicname = "topic://domain:test_topic_send_sub";
		g_queue_rece_index = 0;
		g_topic_rece_index = 0;
		
		int num=10;
		if(argc > 2){
			num = atoi(argv[2]);
		}
		
        if(strcmp(argv[1], "qpsnd") ==0){
            sleep(1);
			test_queue_policy_send(num, policyquque, ack_policyquque);
            sleep(1);
        }
		else if(strcmp(argv[1], "tsnd") ==0){
			sleep(1);
            test_topic_send(num, topicname);
            sleep(1);
        }
        else if(strcmp(argv[1], "qsub") == 0){
            test_queue_subscribe_with_acknowledge(policyquque);
			mq_process_loop();
        }
		else if(strcmp(argv[1], "tsub") == 0){
            test_topic_subscribe(topicname);
			mq_process_loop();
        }
		else if(strcmp(argv[1], "*snd") ==0){
            test_queue_policy_send(num, policyquque, ack_policyquque);
			test_topic_send(num, topicname);
        }
		else if(strcmp(argv[1], "*sub") == 0){
			test_queue_subscribe_with_acknowledge(policyquque);
            test_topic_subscribe(topicname);
			mq_process_loop();
        }
        
        return;
    }
	
	//	test_queue_policy_send_receive_withzip();
	
    test_queue_send_receive();
	sleep(1);
    test_queue_send_receive_mass();
	sleep(1);
    //test_message_encode_decode();
    test_queue_policy_send_receive();
	sleep(1);
    test_queue_policy_send_subscribe();
	sleep(1);
    test_queue_pub_sub();
	sleep(1);
    test_topic_send_receive();
	sleep(1);
    test_topic_pub_sub();


 
//Othere Test
    //mq_subscribe("", topic, TopicMessageListener);
    //mq_send(squeue, squeue, strlen(queueMsg), MQ_MSG_TIMEOUT_DEFAULT);
    //mq_subscribe("", squeue, QueueMessageListener);
    //sleep(10);
    //mq_unsubscribe("", squeue, QueueMessageListener);
    //mq_send(squeue, squeue, strlen(queueMsg), MQ_MSG_TIMEOUT_DEFAULT);
    //mq_send(squeue, squeue, strlen(queueMsg), MQ_MSG_TIMEOUT_DEFAULT);
    //mq_send(topic, topicMsg, strlen(topic), MQ_MSG_TIMEOUT_DEFAULT);
    
    //mq_send(topic, "second message", strlen("second message"), MQ_MSG_TIMEOUT_DEFAULT);
    //sleep(10);
    //mq_unsubscribe("", "topic://startScripts", TopicMessageListener);
    
    //sleep(10);
    //mq_subscribe("", topic, TopicMessageListener);
    //mq_unsubscribe("", topic, TopicMessageListener);
    //mq_send(topic, topicMsg, strlen(topic), MQ_MSG_TIMEOUT_DEFAULT);

//    char* queue = "queue://p2p1";
//    char* queueMsg = "p2p msg";
//    mq_subscribe(NULL, queue, QueueMessageListener);
//    mq_send(queue, queueMsg, strlen(queueMsg), MQ_MSG_TIMEOUT_DEFAULT);
//    mq_unsubscribe(NULL, queue, QueueMessageListener);

    sleep(10);
    mq_release();
    printf("mq_release done\n");
    return 0;
}



