#include "mq_priv.h"
#include "cmn_threadpool.h"
#include "mq_buffer_list.h"
#include "mq_threads_event_processor.h"
#include "mq_map_string_list.h"
#include "mq_map_string_evtitem.h"
#include "mq_cmn_def.h"

static cmn_threadpool*      __g_event_processor_threadPool;
static mq_map_string_list_t*  __g_map_topic_buffer_list;

static mq_map_string_itemevent_t*  __g_map_ack_waiting_event_item;

static void* mq_threads_event_processor_event_handler(void* inp);

void mq_threads_event_processor_init(int numThrd)
{
    MQ_LOG_INFO("mq_threads_event_processor_init: creating process thread pool.");
    __g_event_processor_threadPool = cmn_threadpool_create(numThrd);
    
    MQ_LOG_INFO("mq_threads_event_processor_init: creating event process buffer list.");
    __g_map_topic_buffer_list = mq_map_string_list_create();
    
    MQ_LOG_INFO("mq_threads_event_processor_init: creating event process queue acknowledge list.");
    __g_map_ack_waiting_event_item = mq_map_string_itemevent_create();
}

void mq_threads_event_processor_stop()
{
    if(__g_event_processor_threadPool)
    {
        MQ_LOG_INFO("mq_threads_event_processor_stop: release process thread pool.");
        cmn_threadpool_destroy(__g_event_processor_threadPool);
        
        MQ_LOG_INFO("mq_threads_event_processor_stop: release event process buffer list.");
        mq_map_string_list_release(__g_map_topic_buffer_list);
        
        MQ_LOG_INFO("mq_threads_event_processor_stop: release event process queue acknowledge list.");
        mq_map_string_itemevent_release(__g_map_ack_waiting_event_item);
    }
    
}

void mq_threads_event_processor_add_task(const char* domain, const char* topic, const char* strMsg, const int size, const MessageListener listener, int* outChannelPriority)
{
    if(__g_event_processor_threadPool)
    {
        data_event_processor_entry* pData = NULL;
        
        if(strMsg)
        {
            MQMessage* mqMsg = msg_json_parse(strMsg);
            if(mqMsg)
            {
                if(mqMsg->messageType == MQ_MSG_TYPE_QUEUE_ACK)
                {
                    event_item_entry* pentry = mq_threads_event_processor_get_acknowledge_waiting_evtitem("", topic, mqMsg->id);
                    if(pentry)
                    {
                        cmn_lock_lock(pentry->plock);
                        //pentry->iStatus = mqMsg->payload[0] - '0';
						pentry->iStatus = atoi(mqMsg->payload);
                        cmn_event_trigger(pentry->pevt);
                        cmn_lock_unlock(pentry->plock);
                        MQ_LOG_INFO("mq_threads_event_processor_add_task: acknowledge message received for topic(%s), msgid(%d).", topic, mqMsg->id);
                    }
                    else
                    {
                        MQ_LOG_WARN("mq_threads_event_processor_add_task: acknowledge message received without waiting events (timeout, etc) for topic(%s), msgid(%d).", topic, mqMsg->id);
                    }
                }
                else
                {
                    char* ppl = mqMsg->payload;
					char* unzippeddatabuffer = NULL;
					int ipplsize = strlen(ppl);

					if(mqMsg->rawSize > 0) {  //If the message is zipped
						int idatabuffersize = mqMsg->rawSize < MQ_QUEUE_MAX_SIZE ? mqMsg->rawSize : MQ_QUEUE_MAX_SIZE;
						idatabuffersize = idatabuffersize + 1;
						
						unzippeddatabuffer = (char*) malloc(idatabuffersize);
						int ires = mq_uncompress_buffer(ppl, ipplsize, 1, unzippeddatabuffer, &idatabuffersize);
						if(ires == MQ_OK){
							ppl = unzippeddatabuffer;
							ipplsize = idatabuffersize;
						}
						else{
							MQ_LOG_WARN("mq_threads_event_processor_add_task: unzip data message failure, using initial message as the payload.");
							free(unzippeddatabuffer);
							unzippeddatabuffer = NULL;
							idatabuffersize = 0;
						}
					}

                    int total_size = sizeof(data_event_processor_entry) + ipplsize + MQ_ALIGNMENT_BUFFER_SIZE;
					
                    int appendix_size = 1;      //indicate if there is additional data after string.
                    char* appendix_string = NULL;
                    if( (mqMsg!=NULL) && (mqMsg->channel!= NULL) )
                    {
                        //If the channel has priorty, provide it to the caller, channel priority will be override by message priority
                        //if(outChannelPriority != NULL){
                        //    *outChannelPriority = mqMsg->channel->priority;
                        //}
                        appendix_string=msg_json_get_ack_message(mqMsg);
                        appendix_size += strlen(appendix_string);
                    }
					
                    if(outChannelPriority != NULL){
						*outChannelPriority = mqMsg->priority;
					}
					
                    total_size += appendix_size;
                    
                    pData = (data_event_processor_entry*)malloc(total_size);
                    memset(pData, 0, total_size);
                    
                    memcpy(pData->domain, domain, strlen(domain) +1);
                    memcpy(pData->queueOrTopic, topic, strlen(topic) + 1);
                    memcpy((char*)pData->messageData, ppl, ipplsize + 1);
                    
                    pData->messageSize = ipplsize + 1 + appendix_size; 
                    pData->replySize = appendix_size;
                    pData->messageHandler = listener;

                    if(appendix_string)
                    {
                        char* pack_msg = (char*)pData->messageData + ipplsize + 1; 
                        memcpy(pack_msg, appendix_string, appendix_size -1);
                        
                        MQ_LOG_INFO("mq_threads_event_processor_add_task: data received for domain(%s) topic/queue(%s), size(%d) message[%s %s]",
                                        domain, topic, pData->messageSize, (char*)pData->messageData, pack_msg);
                    }
                    else
                    {
                        MQ_LOG_INFO("mq_threads_event_processor_add_task: data received for domain(%s) topic/queue(%s), size(%d) message(%s)",
                                        domain, topic, pData->messageSize, (char*)pData->messageData);
                    }
                    
                    
                    if( (mqMsg!=NULL) && (mqMsg->channel!= NULL) && (mqMsg->channel->replyMode == MQ_MSG_REPLY_AUTO) )
                    {
                        mq_acknowledge(appendix_string, strlen(appendix_string) + 1, 0, MQ_MSG_TIMEOUT_DEFAULT/1000);
                        MQ_LOG_INFO("mq_threads_event_processor_add_task: automatic acknowledged for domain(%s) topic/queue(%s), ack message(%s)",
                                    mqMsg->fromDomain, mqMsg->channel->replyQueue, appendix_string);
                        pData->messageSize = ipplsize + 1;
                        pData->replySize = 1; // acknowledge is already done, no need to return out;
                    }

                    if(listener == NULL)
                    {
                        MQ_LOG_INFO("mq_threads_event_processor_add_task: response message added to the receive list for domain(%s) topic/queue(%s)", domain, topic);
                        mq_buffer_list_t* plist = mq_map_string_list_add_item_entry(__g_map_topic_buffer_list, pData->queueOrTopic, pData, 0/*__g_mq_max_buffer_size_per_queue*/);
                        
                        //If the list size of larger than max value, decrease the priority
                        if(mq_buffer_list_size(plist) >= mq_config_get_max_buffer_size_per_queue()){
                            if(outChannelPriority != NULL){
                                *outChannelPriority = -1;
                            }
                        }
                    }
                    else
                    {
                        MQ_LOG_INFO("mq_threads_event_processor_add_task: message added to event process pool domain(%s) topic/queue(%s), message(%s)", 
                                                pData->domain, pData->queueOrTopic, pData->messageData);
                        cmn_threadpool_add_task(__g_event_processor_threadPool, mq_threads_event_processor_event_handler, pData);
                    }
                    
                    free(appendix_string);
					
					if(unzippeddatabuffer) free(unzippeddatabuffer);
                }
                msg_destroy(mqMsg);
            }
            else
            {
                MQ_LOG_ERR("mq_threads_event_processor_add_task: invalid mq message received (jason parse failed).");
            }
        }
        else
        {
            MQ_LOG_ERR("mq_threads_event_processor_add_task: skipped empty message.");
        }

    }
    else
    {
        MQ_LOG_ERR("mq_threads_event_processor_add_task: __g_event_processor_threadPool is not initialized.");
    }
    
}

static void* mq_threads_event_processor_event_handler(void* inp){
    data_event_processor_entry* prm = (data_event_processor_entry*)inp;

    if(!prm) return NULL;

    MQ_LOG_DEBUG("mq_threads_event_processor_event_handler: custom call back is being called...");
    char* pmsgdata = prm->messageData;
    int imsgsize = prm->messageSize - prm->replySize;
    
    char* preplydata = NULL;
    int ireplysize = 0;
    if(prm->replySize > 1){
        preplydata = prm->messageData + imsgsize;
        ireplysize = prm->replySize;
    }
    
    prm->messageHandler(pmsgdata, imsgsize, preplydata, ireplysize);
    
    free(prm);

    return NULL;
}

void* mq_threads_event_processor_get_response_buffers(const char* domain, const char* topic)
{
    mq_buffer_list_t* plist = NULL;
    plist = mq_map_string_list_get_item(__g_map_topic_buffer_list, topic);
    MQ_LOG_DEBUG("mq_threads_event_processor_get_response_buffers: returned(%s)", (plist==NULL ? "NULL" : "List"));
    
    return plist;
}

void* mq_threads_event_processor_create_response_buffers(const char* domain, const char* topic)
{
    MQ_LOG_DEBUG("mq_threads_event_processor_create_response_buffers: for domain(%s), queue/topic(%s)", domain, topic);
    return mq_map_string_list_add_item_entry(__g_map_topic_buffer_list, topic, NULL, 0);
}

void* mq_threads_event_processor_get_acknowledge_waiting_evtitem(const char* domain, const char* topic, long idMsg)
{
    event_item_entry* pevtItem = NULL;
    
    char ack_messageid[MQ_ACK_MESSAGE_ID_SIZE];
    snprintf(ack_messageid, MQ_ACK_MESSAGE_ID_SIZE, "%s-%ld", topic, idMsg);
    
    pevtItem = mq_map_string_itemevent_get_item(__g_map_ack_waiting_event_item, ack_messageid);
    
    MQ_LOG_DEBUG("mq_threads_event_processor_get_acknowledge_waiting_evtitem: domain(%s), queue/topic(%s), idMsg(%d), returned(%s)", 
                    domain, topic, idMsg, (pevtItem==NULL ? "NULL" : "EventItem"));
    return pevtItem;
}

void* mq_threads_event_processor_create_acknowledge_waiting_evtitem(const char* domain, const char* topic, long idMsg)
{
    char ack_messageid[MQ_ACK_MESSAGE_ID_SIZE];
    snprintf(ack_messageid, MQ_ACK_MESSAGE_ID_SIZE, "%s-%ld", topic, idMsg);
    MQ_LOG_DEBUG("mq_threads_event_processor_create_acknowledge_waiting_evtitem: for domain(%s), queut/topic(%s), idMsg(%d)", domain, topic, idMsg);
    return mq_map_string_itemevent_add_item(__g_map_ack_waiting_event_item, ack_messageid, NULL);
}

void mq_threads_event_processor_release_acknowledge_waiting_evtitem(const char* domain, const char* topic, long idMsg)
{
    char ack_messageid[MQ_ACK_MESSAGE_ID_SIZE];
    snprintf(ack_messageid, MQ_ACK_MESSAGE_ID_SIZE, "%s-%ld", topic, idMsg);    
    mq_map_string_itemevent_remove_item(__g_map_ack_waiting_event_item, ack_messageid);
    
    MQ_LOG_DEBUG("mq_threads_event_processor_release_acknowledge_waiting_evtitem: for domain(%s), queut/topic(%s), idMsg(%d)", domain, topic, idMsg);
}
