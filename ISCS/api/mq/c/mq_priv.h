#ifndef MQ_PRIV_H
#define MQ_PRIV_H

#include "hiredis.h"
#include "async.h"
#include "c_vector.h"
#include "cmn_lock.h"
#include "mq.h"

//每个连接的最大共享会话数量
//#define MQ_MAX_SESSION_PER_CONNECTION    1

#define MQ_HEART_BEAT_DEFAULT            500

// IP Size
#define MQ_IP_SIZE                       20
#define MQ_PORT_SIZE                     10
#define MQ_PROTOCOL_SIZE                 10
#define MQ_USER_SIZE                     10
#define MQ_PASSWD_SIZE                   32
#define MQ_DEFAULT_SIZE                  256
#define MQ_NUM_HOST_INTERFACES           2

#define MQ_ALIGNMENT_BUFFER_SIZE 		 4
// command definition
#define MQ_AUTH                  "auth"

//Topic/Msg Queue 1.0, no domain
#define MQ_SUBSCRIBE             "subscribe"
#define MQ_UNSUBSCRIBE           "unsubscribe"
#define MQ_PUBLISH               "publish"

//Topic/Msg Queue 1.0, no domain
#define MQ_QUEUE_SEND            "rpush"
#define MQ_QUEUE_RECEIVE         "blpop"

#define MQ_QUEUE_PSEND           "zadd"
#define MQ_QUEUE_PRECEIVE        "bzpopmin"

//Topic 2.0,support domain
#define MQ_TSUBSCRIBE            "tsubscribe"
#define MQ_TUNSUBSCRIBE          "tunsubscribe"
#define MQ_TPUBLISH              "tpublish"

//Msg Queue 2.0,support domain
#define MQ_QSUBSCRIBE            "qsubscribe"
#define MQ_QUNSUBSCRIBE          "qunsubscribe"
#define MQ_QPUBLISH              "qpublish"

#define NAME_QUEUE_ADMIN         "mq_adminqueue"
#define NAME_TOPIC_ADMIN         "mq_admintopic"
#define NAME_QUEUE_ALLQUEUE      "mq_allqueues"
#define MQ_ADD_QUEUE_ALLQUEUE    MQ_QUEUE_PSEND

typedef int bool;

typedef struct MQHost {
    char protocol[MQ_PROTOCOL_SIZE];       //reserved
    char ip[MQ_NUM_HOST_INTERFACES][MQ_IP_SIZE];
    int  port;
    int  poolsize;
    int  timeout;
    int  beat_internal;

    char user[MQ_USER_SIZE];
    char passwd[MQ_PASSWD_SIZE];

} MQHost;

typedef struct MQConnection {
    MQHost host;

    int conn_type;              //1: synchronized, 0: async
    redisContext* redisClient;
    redisAsyncContext* redisAsyncClient;

    int heartbeat; //milli second

    int refSessions; // connection shared count by session, default 1, Reserved
} MQConnection;

typedef struct MQPooledConnectionFactory {
//    char* hosts;
//    char* user;
//    char* passwd;
//    int poolSize;

    //c_vector connections;           //<MQonnection>
    //MQConnection* async_connection;
    c_vector    mq_hosts;
    int         master_host_index;
} MQPooledConnectionFactory;


typedef int (*MessageListener)(void* buffer, const int size, void* replybuff, const int replysize);

// session在多线程下操作，必须同步
typedef struct MQSafeSession {
    //MQConnection*    masterConn; //会话：线程：连接=1:1:1，即session对connection的shareholder=1，独占
    //pthread_mutex_t  monoLock;
    //MQConnection*    slaveConn; // h/a master salve switch

    //MessageListener  subCallback; //session in subscribed, no heartbeat need
    //char*            subOpt;
    //char*            domain;
    //char*            queueOrTopic;
    cmn_lock*          session_lock;
    c_vector           session_connections;
    int                session_type;              //1: synchronized, 0: async;
} MQSafeSession;

//reserved
//typedef struct MQMessageConsumer {
//    MQChannel *channel;


//} MQMessageConsumer;

// reserved
//typedef struct MQMessageProducer {
//    MQChannel *channel;
    

//} MQMessageProducer;

typedef struct MQMessage {
    int     version;        //消息包版本
    int     messageType;    //消息包版本
    int     totalSize;      
    long    id;             //MQ自动生成uuid的hashcode
    long    timestamp;      //发送时间
    int     ttl;            //TTL
    int     priority;       //优先级
    char    toDomain[MQ_DOMAIN_SIZE];
    char    fromDomain[MQ_DOMAIN_SIZE];   //消息起源域唯一标识
    char    queueOrTopic[MQ_TOPIC_SIZE];  //消息队列标识
    MQChannel* channel;     //点对点消息专用, it is allocated after the payload
    int	 rawSize;					//If message is compressed, the size before compression, otherwise it is 0
	int     payloadSize;           //消息大小
    char    payload[0];     //消息载体
} MQMessage;

// connection
int mq_conn_init();
int mq_conn_release();
//MQPooledConnectionFactory* mq_conn_get_factory();
//c_vector mq_conn_pool();
c_vector mq_conn_hosts();
int mq_conn_master_index();
int mq_conn_recover(MQConnection* conn);

//MQConnection* mq_async_conn();
MQConnection* mq_conn_create_connection(int indexHost, int isSync);
int mq_conn_destroy(MQConnection* conn);

// session
int mq_sessn_init();
int mq_sessn_release();
MQSafeSession* mq_sessn_open(int isSync);
int mq_sessn_close(MQSafeSession* session);
int mq_sessn_is_connection_valid(MQSafeSession* session, int isSync);
void * mq_session_get_master_connection(MQSafeSession* sess, int isSync);
int mq_sessn_send(MQSafeSession* session, const char* opt, const char* queueOrTopic, const char* message, const int timeout, const int parm1);
void* mq_sessn_execute(MQSafeSession* session, const char* opt, const char* queueOrTopic, const int timeout);
//int mq_sessn_subscribe(MQSafeSession* session, const char* opt, const char* domain, const char* queueOrTopic, const MessageListener listener);
//int mq_sessn_unsubscribe(MQSafeSession* session, const char* opt, const char* domain, const char* queueOrTopic, const MessageListener listener);

//MQSafeSession* mq_sessn_async_get();
void mq_sessn_attach_event_loop(MQSafeSession* session, void* eb);
int mq_sessn_async_subscribe(MQSafeSession* session, const char* opt, const char* domain, const char* queueOrTopic,  void* privData);

// pubsub
int mq_pubsub_receive(const char* domain, const char* queueOrTopic, const long timeout, char** buffer, int* size);
int mq_pubsub_send_topic(const char* domain, const char* Topic, const char* payload, const int size, const long timeout);
int mq_pubsub_send_queue(const char* domain, const char* queueOrTopic, const char* payload, const int size, const long timeout, MQChannel* pchn, int msgType, int priority);
int mq_pubsub_receive_topic(const char* domain, const char* queueOrTopic, const long timeout, char** buffer, int* size);
int mq_pubsub_receive_queue(const char* domain, const char* queueOrTopic, const long timeout, char** buffer, int* size, char** replybuffer, int* replysize);
int mq_pubsub_subscribe_topic(const char* domain, const char* topic, const MessageListener listener);
int mq_pubsub_unsubscribe_topic(const char* domain, const char* topic, const MessageListener listener);
int mq_pubsub_subscribe_queue(const char* domain, const char* topic, const MessageListener listener);
int mq_pubsub_unsubscribe_queue(const char* domain, const char* topic, const MessageListener listener);
int mq_pubsub_acknowledge_queue(const char* replybuffer, const int replybuffersize, const int ackstatus, const long timeout);
int mq_pubsub_trigger_receive_admintopicqueue(const char* domain, int isTQtype);


//utils
bool mq_is_topic(const char* dest);
bool mq_is_queue(const char* dest);
long mq_msg_id_generate();
const char * mq_extract_queue_name(const char* fullQN);
const char * mq_get_local_domain();
int mq_adjust_priority_with_timestamp(const char* queuename, int ipriority);
int string_comparer(void * x, void * y);

int mq_compress_buffer(const unsigned char* rawData, const int inrawlen, bool based64Enc, unsigned char** outDataBuff, int* outlen);
int mq_uncompress_buffer(const unsigned char* incompressedData, int incompressedlen, bool based64Enc, unsigned char* outuncompressedDataBuffer, int* uncompressedlen /*In/Out*/);

char* format_queue_topic_name(const char* domain, const char* tqname, char* databuffer, int lenbuffer);

//MQMessage
MQMessage* mq_msg_create(int msgType, const char* domain, const char* topic, const char* payload, const int size, MQChannel * chnn, const int priority);
void msg_destroy(MQMessage* topic);
char* msg_json_serialize(const MQMessage* msg);
MQMessage* msg_json_parse(const char* jstring);
char* msg_json_get_ack_message(const MQMessage* msg);

#endif //MQ_PRIV_H
