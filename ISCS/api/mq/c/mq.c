#include <pthread.h>

#include "mq_constant.h"
#include "mq.h"
#include "mq_priv.h"
#include "mq_threads_topic_subscribe.h"
#include "mq_threads_queue_receive.h"
#include "mq_threads_send.h"
#include "mq_threads_event_processor.h"
#include "mq_priv.h"
#include "mq_cmn_def.h"

#define REFORMAT_DOMAIN_TQNAME(_domain, _tqname) \
        char __queueOrTopic_databuffer[MAXLEN_QUEUE_TOPIC_NAME]; \
        _tqname = format_queue_topic_name(_domain, _tqname, __queueOrTopic_databuffer, MAXLEN_QUEUE_TOPIC_NAME); \
        if(_tqname == __queueOrTopic_databuffer) { MQ_LOG_DEBUG("queue/topic name is adjusted to: %s.", _tqname); }

static pthread_mutex_t g_initLock = PTHREAD_MUTEX_INITIALIZER;
static int g_mqRef = 0;

int mq_init(const char* domain){
    MQ_LOG_DEBUG("mq_init enter.");
    int iret = MQ_OK;
    
    pthread_mutex_lock(&g_initLock);

    if (g_mqRef++ == 0) 
    {

		mq_config_log_initialization();
		
        if(mq_conn_init() != MQ_OK)    
        {
            MQ_LOG_ERR("mq_conn_init failure, mq_init return with error.");
            iret = MQ_ERROR;
        }

        if( (iret==MQ_OK) && (mq_sessn_init() != MQ_OK) )   
        {
            MQ_LOG_ERR("mq_sessn_init failure, mq_init return with error.");
            iret = MQ_ERROR;
        }
        
        if(iret==MQ_OK)
        {
            MQ_LOG_INFO("mq event processor initialization...");
            mq_threads_event_processor_init(mq_config_get_int(MQ_CONF_NUM_EVENT_PROCESS_POOL_SIZE, MQ_EVENT_PROCESSOR_NUM));

            int mq_disable_topic = mq_config_get_int(MQ_CONF_DISABLE_TOPIC_CHANNEL, 0);
            int mq_disable_queue = mq_config_get_int(MQ_CONF_DISABLE_QUEUE_CHANNEL, 0);
            
            if(mq_disable_topic > 0)
            {
                MQ_LOG_WARN("mq topic channel is disabled.");
            }
            else
            {
                
                MQ_LOG_INFO("mq topic subscribe thread initialization and starting...");
                mq_threads_topic_subscribe_init();
                mq_threads_topic_subscribe_start();
            }
            

            if(mq_disable_queue > 0)
            {
                MQ_LOG_WARN("mq queue channel is disabled.");
            }
            else
            {
                MQ_LOG_INFO("mq queue receive thread initialization and starting...");
                mq_threads_queue_receive_init();
                mq_threads_queue_receive_start();
            }
            
            if( (mq_disable_topic == 0) || (mq_disable_queue == 0) )
            {
                MQ_LOG_INFO("mq queue send thread(s) initialization and starting...");
                mq_threads_send_init(mq_config_get_int(MQ_CONF_NUM_QUEUE_SEND_THREADS, 1));
                mq_threads_send_start();
            }
        }
    }

    pthread_mutex_unlock(&g_initLock);

    MQ_LOG_DEBUG("mq_init exit.");
    return iret;
}

int mq_release(){
    MQ_LOG_DEBUG("mq_release enter.");

    pthread_mutex_lock(&g_initLock);

    if(--g_mqRef == 0) {
        MQ_LOG_INFO("mq processing threads stopping...");
        mq_threads_queue_receive_stop();
        mq_threads_topic_subscribe_stop();
        mq_threads_send_stop();
        mq_threads_event_processor_stop();        
        
        MQ_LOG_INFO("mq sessions release...");
        mq_sessn_release();
        
        MQ_LOG_INFO("mq connections release...");
        mq_conn_release();
        
        mq_logger_destory();
		
		release_mq_conf();
    }

    pthread_mutex_unlock(&g_initLock);

    MQ_LOG_DEBUG("mq_release exit.");
    return MQ_OK;
}

//pubsub->session->connection
int mq_send(const char* domain, const char* queueOrTopic, const char* payload, const int size, const long timeout){
    if(!queueOrTopic || !payload)
        return MQ_ILLEGAL_PARAM;

    int iret = MQ_OK;
    MQ_LOG_INFO("mq_send: domain:%s, queue/topic:%s, payload:%s, size:%d, timeout:%d.", domain, queueOrTopic, payload, size, timeout);
    if(mq_is_queue(queueOrTopic)) {
        REFORMAT_DOMAIN_TQNAME(domain, queueOrTopic);
        iret = mq_pubsub_send_queue(domain, queueOrTopic, payload, size, timeout, NULL, MQ_MSG_TYPE_QUEUE_NORMAL, MQ_MSG_PRIORITY_DEFAULT);
    } else {
        REFORMAT_DOMAIN_TQNAME(domain, queueOrTopic);
        iret = mq_pubsub_send_topic(domain, queueOrTopic, payload, size, timeout);
    }
    MQ_LOG_INFO("mq_send: domain:%s, queue/topic:%s done. size:%d. result: %d. ", domain, queueOrTopic, size, iret);
    return iret;
}

//pubsub->session->connection
int mq_receive(const char* domain, const char* queueOrTopic, const long timeout, char** buffer, int* size, char** replybuffer, int* replysize){
    if(!queueOrTopic || !buffer || !size)
        return MQ_ILLEGAL_PARAM;

    int iret = MQ_OK;
    MQ_LOG_INFO("mq_receive: domain:%s, queue/topic:%s, timeout:%d.", domain, queueOrTopic, timeout);
    if(mq_is_queue(queueOrTopic)) 
    {
        REFORMAT_DOMAIN_TQNAME(domain, queueOrTopic);
        iret = mq_pubsub_receive_queue(domain, queueOrTopic, timeout, buffer, size, replybuffer, replysize);
    } 
    else 
    {
        REFORMAT_DOMAIN_TQNAME(domain, queueOrTopic);
        iret = mq_pubsub_receive_topic(domain, queueOrTopic, timeout, buffer, size);
        if(replybuffer) *replybuffer = NULL;
        if(replysize) *replysize = 0;
    }
    MQ_LOG_INFO("mq_receive: domain:%s, queue/topic:%s done. result: %d", domain, queueOrTopic, iret);
    return iret;
}

int mq_received_free(char *buffer){
    if(buffer) free(buffer);
    
    return MQ_OK;
}

//pubsub->session->connection
int mq_subscribe(const char* domain, const char* queueOrTopic, const MessageListener listener){
    if(!queueOrTopic || !listener)
        return MQ_ILLEGAL_PARAM;

    int iret = MQ_OK;
    MQ_LOG_INFO("mq_subscribe: domain:%s, queue/topic:%s", domain, queueOrTopic);
    if(mq_is_queue(queueOrTopic)) {
        REFORMAT_DOMAIN_TQNAME(domain, queueOrTopic);
        iret = mq_pubsub_subscribe_queue(domain, queueOrTopic, listener);
    } else {
        REFORMAT_DOMAIN_TQNAME(domain, queueOrTopic);
        iret = mq_pubsub_subscribe_topic(domain, queueOrTopic, listener);
    }
    MQ_LOG_INFO("mq_subscribe: domain:%s, queue/topic:%s done, result: %d.", domain, queueOrTopic, iret);
    return iret;
}

//pubsub->session->connection
int mq_unsubscribe(const char* domain, const char* queueOrTopic, const MessageListener listener){
    if(!queueOrTopic || !listener)
        return MQ_ILLEGAL_PARAM;

    int iret = MQ_OK;
    MQ_LOG_INFO("mq_unsubscribe: domain:%s, queue/topic:%s", domain, queueOrTopic);
    if(mq_is_queue(queueOrTopic)) {
        REFORMAT_DOMAIN_TQNAME(domain, queueOrTopic);
        iret = mq_pubsub_unsubscribe_queue(domain, queueOrTopic, listener);
    } else {
        REFORMAT_DOMAIN_TQNAME(domain, queueOrTopic);
        iret = mq_pubsub_unsubscribe_topic(domain, queueOrTopic, listener);
    }
    MQ_LOG_INFO("mq_unsubscribe: domain:%s, queue/topic:%s done, result: %d.", domain, queueOrTopic, iret);
    return iret;
}

int mq_process_loop() {
    mq_threads_topic_subscribe_join();
    mq_threads_send_join();
    
    return MQ_OK;
}


MQChannel* mq_channel_create(const char* domain, const char* queue, int priority, int replyMode, const char* replyQueue) {
    if(!queue || !replyQueue)
        return NULL;

    int isize = sizeof(MQChannel);
    MQChannel* pChnn = (MQChannel*)malloc(isize);
    memset(pChnn, 0, sizeof(isize));
    
    strncpy(pChnn->domain, domain, MQ_DOMAIN_SIZE);

    char __queueOrTopic_databuffer[MAXLEN_QUEUE_TOPIC_NAME];
    
    queue = format_queue_topic_name(domain, queue, __queueOrTopic_databuffer, MAXLEN_QUEUE_TOPIC_NAME);
    strncpy(pChnn->queName, queue, MQ_TOPIC_SIZE);
    
    replyQueue = format_queue_topic_name(domain, replyQueue, __queueOrTopic_databuffer, MAXLEN_QUEUE_TOPIC_NAME);
    strncpy(pChnn->replyQueue, replyQueue, MQ_TOPIC_SIZE);
    pChnn->priority = priority;
    pChnn->replyMode = replyMode;
    
    return pChnn;
}

int mq_channel_destroy(MQChannel* channel)
{
    if(channel)
    {
        free(channel);
    }
    
    return MQ_OK;
}

int mq_policy_send(MQChannel* channel, const char* payload, const int size, const long timeout, const int priority)
{
    int ires = MQ_OK;
    if(!channel || !payload)
        return MQ_ILLEGAL_PARAM;

    MQ_LOG_INFO("mq_policy_send: domain:%s, queue/topic:%s, payload:%s, size:%d, timeout:%d. priority:%d", channel->domain, channel->queName, payload, size, timeout, priority);
    if( (channel != NULL) && (mq_is_queue(channel->queName)) ) {
        ires = mq_pubsub_send_queue(channel->domain, channel->queName, payload, size, timeout, channel, MQ_MSG_TYPE_QUEUE_NORMAL, priority);
        
        MQ_LOG_INFO("mq_policy_send: domain:%s, queue/topic:%s, payload:%s, size:%d, timeout:%d, priority:%d, res:%d", channel->domain, channel->queName, payload, size, timeout, priority, ires);
        return ires;
    } 
    else 
    {
        return MQ_ILLEGAL_PARAM; //policy send only for queue 
    }
}

int mq_acknowledge(const char* replybuffer, const int replybuffersize, const int ackstatus, const long timeout)
{
    if(!replybuffer)
        return MQ_ILLEGAL_PARAM;
    int ires = MQ_OK;
    MQ_LOG_INFO("mq_acknowledge: replybuffer:%s, replybuffersize:%d, ackstatus:%d, timeout:%d", replybuffer, replybuffersize, ackstatus, timeout);
    ires = mq_pubsub_acknowledge_queue(replybuffer, replybuffersize, ackstatus, timeout);
    MQ_LOG_INFO("mq_acknowledge: replybuffer:%s, replybuffersize:%d, ackstatus:%d, timeout:%d, ires:%d", replybuffer, replybuffersize, ackstatus, timeout, ires);
    return ires;
}
