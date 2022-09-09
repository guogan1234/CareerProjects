#include <unistd.h>
#include <pthread.h>

#include "cmn_sched.h"
#include "cmn_threadpool.h"
#include "c_list.h"

#include "mq_constant.h"
#include "mq_priv.h"
#include "adapters/ae.h"
#include "mq_threads_topic_subscribe.h"
#include "mq_threads_event_processor.h"
#include "mq_cmn_def.h"

static void connectCallback(const redisAsyncContext *c, int status) 
{
    if (status != REDIS_OK) 
    {
        MQ_LOG_ERR("mq_session_async(connectCallback): status(%d), error(%s).", status, (c == NULL ? "NULL" : c->errstr));
        return;
    }
    else
    {
        MQ_LOG_INFO("mq_session_async(connectCallback): connected.");
    }
    
}

static void disconnectCallback(const redisAsyncContext *c, int status) 
{
    if (status != REDIS_OK) 
    {
        MQ_LOG_ERR("mq_session_async(disconnectCallback): status(%d), error(%s).", status, (c == NULL ? "NULL" : c->errstr));
        return;
    }
    else
    {
        MQ_LOG_INFO("mq_session_async(disconnectCallback): disconnected.");
    }
}

static void subCallback(redisAsyncContext *c, void *r, void *priv) {
    redisReply *reply = r;
    if (reply == NULL) return;
    if ( reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 ) 
    {
        if ( (strcmp( reply->element[0]->str, "subscribe" ) != 0)  &&
                    (strcmp( reply->element[0]->str, "unsubscribe" ) != 0) )
        {
            //printf( "Received[%s] channel %s: %s\n", (char*)priv, reply->element[1]->str, reply->element[2]->str );
            if(priv)
            {
                data_topic_subscribe_entry* pData = (data_topic_subscribe_entry*)priv;
                
                MQ_LOG_INFO("mq_session_async(subCallback): adding message to event processor: domain:%s, topic:%s, message: %s.", 
                            pData->domain, reply->element[1]->str, reply->element[2]->str);

                mq_threads_event_processor_add_task(pData->domain,
                                                    reply->element[1]->str, 
                                                    reply->element[2]->str, 
                                                    strlen(reply->element[2]->str), 
                                                    pData->messageHandler,
                                                    NULL);
            }
            else
            {
                MQ_LOG_DEBUG("mq_session_async(subCallback): empty priv data found for topic:%s, message: %s. (might be admin topic...)", reply->element[1]->str, reply->element[2]->str); 
            }
        }
        else
        {
            MQ_LOG_WARN("mq_session_async(subCallback): unexpected command reply recieved, %s.", reply->element[0]->str); 
        }
    }
    else
    {
        MQ_LOG_WARN("mq_session_async(subCallback): unexpected reply recieved, %s.", reply->str); 
    }
    
}


void mq_sessn_attach_event_loop(MQSafeSession* session, void * eb)
{
    redisAsyncContext* async_conn = mq_session_get_master_connection(session, 0);
    
    redisAeAttach((aeEventLoop*)eb, async_conn);
    redisAsyncSetConnectCallback(async_conn,connectCallback);
    redisAsyncSetDisconnectCallback(async_conn,disconnectCallback);
}

int mq_sessn_async_subscribe(MQSafeSession* session, const char* opt, const char* domain, const char* queueOrTopic, void* privData)
{
	int istatus = MQ_OK;
    cmn_lock_lock(session->session_lock);
    redisAsyncContext* async_conn = mq_session_get_master_connection(session, 0);
    if( (async_conn) && (async_conn->err==REDIS_OK) )
    {
        char cmd_buff[1024];
        memset(cmd_buff, 0, sizeof(1024));
        snprintf(cmd_buff, 1023, "%s %s", opt, queueOrTopic);
        
        MQ_LOG_DEBUG("mq_sessn_async_subscribe: command sent: %s",cmd_buff);
        istatus = redisAsyncCommand(async_conn, subCallback, privData, cmd_buff);
        
        if ( istatus != REDIS_OK )
        {
            MQ_LOG_DEBUG("mq_sessn_async_subscribe: redisAsyncCommand: %s, reply: %d, error: %d - %s.", cmd_buff, istatus, async_conn->err, async_conn->errstr);
        }
        else
        {
            MQ_LOG_DEBUG("mq_sessn_async_subscribe: redisAsyncCommand. OK");
        }
    }
    else
    {
        MQ_LOG_ERR("mq_sessn_async_subscribe: invalid redisContext for command execution. error: %s", (async_conn==NULL ? "async_conn==NULL" : async_conn->errstr));
    }
	cmn_lock_unlock(session->session_lock);

    return istatus;
}

