//发送接收消息，此层代码建议不直接调用redis接口，而是通过session封装接口，防止session中的masterConn漂移

#include <time.h>

#include "parson.h"

#include "mq_constant.h"
#include "mq_priv.h"
#include "mq_threads_topic_subscribe.h"
#include "mq_threads_send.h"
#include "mq_threads_queue_receive.h"
#include "mq_map_string_list.h"
#include "mq_threads_event_processor.h"
#include "mq_cmn_def.h"

int mq_pubsub_subscribe_topic(const char* domain, const char* topic, const MessageListener listener){
    int result = MQ_OK;
    
    MQ_LOG_INFO("mq_pubsub_subscribe_topic: add subscribe to topic subscribe list for domain:%s, topic:%s", domain, topic);
    mq_threads_topic_subscribe_add_entry(MQ_SUBSCRIBE, domain, topic, listener);
	
	//unblock the aeMain by sending one messsage to admin topic
	mq_pubsub_trigger_receive_admintopicqueue(domain, MQ_TQTYPE_TOPIC);

    return result;
}

int mq_pubsub_unsubscribe_topic(const char* domain, const char* topic, const MessageListener listener){
    int result = MQ_OK;

    MQ_LOG_INFO("mq_pubsub_unsubscribe_topic: add unsubscribe to topic subscribe list for domain:%s, topic:%s", domain, topic);
    mq_threads_topic_subscribe_add_entry(MQ_UNSUBSCRIBE, domain,  topic, listener);
	
	//unblock the aeMain by sending one messsage to admin topic
	mq_pubsub_trigger_receive_admintopicqueue(domain, MQ_TQTYPE_TOPIC);

    return result;
}

int mq_pubsub_subscribe_queue(const char* domain, const char* queuename, const MessageListener listener){
    int result = MQ_OK;
    
    MQ_LOG_INFO("mq_pubsub_subscribe_queue: add subscribe to queue subscribe list for domain:%s, queue:%s", domain, queuename);
    char * popt = (mq_config_priority_supported() > 0) ? MQ_QUEUE_PSEND : MQ_QUEUE_SEND;
    result = mq_threads_queue_subscribe_add_entry(popt, domain, queuename, listener);
	
	//If it is a new queue, send a message to adminqueue to unblock the redis command call.
	if(result == MQ_OK){
		mq_pubsub_trigger_receive_admintopicqueue(domain, MQ_TQTYPE_QUEUE);
	}

    return result;
}

int mq_pubsub_unsubscribe_queue(const char* domain, const char* queuename, const MessageListener listener){
    int result = MQ_OK;
    
    MQ_LOG_INFO("mq_pubsub_unsubscribe_queue: add unsubscribe to queue subscribe list for domain:%s, queue:%s", domain, queuename);
    char * popt = (mq_config_priority_supported() > 0) ? MQ_QUEUE_PSEND : MQ_QUEUE_SEND;
    mq_threads_queue_subscribe_remove_entry(popt, domain, queuename, listener);
    
    return result;
}

int mq_pubsub_receive_topic(const char* domain, const char* queueOrTopic, const long timeout, char** buffer, int* size) {
    
    mq_buffer_list_t* pResponseBufferList = mq_threads_event_processor_get_response_buffers(domain, queueOrTopic);
    if(pResponseBufferList == NULL)
    {
        MQ_LOG_INFO("mq_pubsub_receive_topic: No existing response buffers, creating new for topic with domain:%s, topic:%s", domain, queueOrTopic);
        pResponseBufferList = mq_threads_event_processor_create_response_buffers(domain, queueOrTopic);
        mq_pubsub_subscribe_topic(domain, queueOrTopic, NULL);
    }
    
    // wait until get reply here
    data_event_processor_entry* pResponseData = mq_buffer_list_get_item_timeout(pResponseBufferList, timeout);
    if(pResponseData)
    {
        *size = pResponseData->messageSize - pResponseData->replySize;
        *buffer = malloc(*size);
        memset(*buffer, 0, *size); 
        memcpy(*buffer, pResponseData->messageData, (*size));     
        
        free(pResponseData);
        MQ_LOG_INFO("mq_pubsub_receive_topic: topic data received for domain:%s, topic:%s, size(%d), message: %s", domain, queueOrTopic, *size, *buffer);
    }
    else
    {
        MQ_LOG_INFO("mq_pubsub_receive_topic: topic data received for domain:%s, topic:%s, No Data returned", domain, queueOrTopic);
        *size = 0;
    }

    return MQ_OK;
}

int mq_pubsub_receive_queue(const char* domain, const char* queueOrTopic, const long timeout, char** buffer, int* size, char** replybuffer, int* replysize) {
    
    mq_buffer_list_t* pResponseBufferList = mq_threads_event_processor_get_response_buffers(domain, queueOrTopic);
    
    if(pResponseBufferList == NULL)
    {
        MQ_LOG_INFO("mq_pubsub_receive_queue: No existing response buffers, creating new for queue with domain:%s, queue:%s", domain, queueOrTopic);
        pResponseBufferList = mq_threads_event_processor_create_response_buffers(domain, queueOrTopic);
        mq_pubsub_subscribe_queue(domain, queueOrTopic, NULL);
    }
    
    // wait until get reply here
    data_event_processor_entry* pResponseData = mq_buffer_list_get_item_timeout(pResponseBufferList, timeout);
    if(pResponseData)
    {
        *size = pResponseData->messageSize - pResponseData->replySize;
        *buffer = malloc(*size + 1);
        memset(*buffer, 0, *size + 1); 
        memcpy(*buffer, pResponseData->messageData, (*size)); 

        if(pResponseData->replySize > 1){
            *replysize = pResponseData->replySize;
            *replybuffer = malloc(*replysize + 1);
            memset(*replybuffer, 0, *replysize + 1); 
            char * preply = pResponseData->messageData + (pResponseData->messageSize - pResponseData->replySize);
            memcpy(*replybuffer, preply, (*replysize));     
        }
        else{
            if(replybuffer) *replybuffer = NULL;
            if(replysize) *replysize = 0;
        }
        
        free(pResponseData);
        MQ_LOG_INFO("mq_pubsub_receive_queue: queue data received for domain:%s, topic:%s, size(%d), message: %s", domain, queueOrTopic, *size, *buffer);
        
        int resetres = mq_threads_queue_reset_queue_priority(queueOrTopic);
		if(resetres == MQ_OK){
			mq_pubsub_trigger_receive_admintopicqueue(domain, MQ_TQTYPE_QUEUE);
		}
    }
    else
    {
        MQ_LOG_INFO("mq_pubsub_receive_queue: queue data received for domain:%s, topic:%s, No Data returned", domain, queueOrTopic);
        *size = 0;
    }
    
    return MQ_OK;
}

int mq_pubsub_send_queue(const char* domain, const char* queueOrTopic, const char* payload, const int size, const long timeout, MQChannel* pchn, int msgType, int priority) {
    int result = MQ_OK;
	
	if(size > MQ_QUEUE_MAX_SIZE)
		return MQ_SYSTEM_ERROR_OVER_SIZE;

	char* msgload = NULL;
	int imsgsize = 0;
	if( (pchn!= NULL) && (pchn->zipMode > 0) ){
		int ires = mq_compress_buffer(payload, size, 1, &msgload, &imsgsize);
		if( ires != MQ_OK){
			MQ_LOG_WARN("mq_pubsub_send_queue: compress raw message failure, message is not been compressed now. ");
			if(msgload) free(msgload);
			
			//reset the message to the raw message if zip failure
			msgload = payload;
			imsgsize = size;
		}
	}
	else{
		msgload = payload;
		imsgsize = size;
	}
	
    //MQMessage* queueMsg = mq_msg_create(msgType, domain, queueOrTopic, payload, size, pchn, priority);
	MQMessage* queueMsg = mq_msg_create(msgType, domain, queueOrTopic, msgload, imsgsize, pchn, priority);
	
	if( (queueMsg) && (msgload != payload) ){ 	//message was zipped
		queueMsg->rawSize = size;
		free(msgload); 											//free the zipped data buffer
	}

    // to json string
    char* message = msg_json_serialize(queueMsg);
    char * popt = (mq_config_priority_supported() > 0) ? MQ_QUEUE_PSEND : MQ_QUEUE_SEND;

    if( (pchn) && (pchn->replyMode > MQ_MSG_REPLY_NO) )
    {
        event_item_entry* pentry = mq_threads_event_processor_create_acknowledge_waiting_evtitem(domain, pchn->replyQueue, queueMsg->id);
        if(pentry)
        {
            MQ_LOG_INFO("mq_pubsub_send_queue: adding item to sending queue for domain:%s, queue:%s, message: %s, priority: %d.", domain, queueOrTopic, message, priority);
            result = mq_threads_send_add_entry(domain, queueMsg->id, popt, queueOrTopic, message, timeout, priority);
            if(result == MQ_OK) {
				mq_pubsub_subscribe_queue("", pchn->replyQueue, NULL);
           
				if(cmn_event_timedwait(pentry->pevt, timeout) == 0)
				{
					result = MQ_NACK_TIMEOUT;
					MQ_LOG_INFO("mq_pubsub_send_queue: acknowledge timeout for domain:%s, queue:%s.", domain, queueOrTopic);
				}
				else
				{
					if(pentry->iStatus == 0){
						result = MQ_OK;
					}
					else {
						result = MQ_NACK_STATUS_BASE | pentry->iStatus;
					}
					MQ_LOG_INFO("mq_pubsub_send_queue: acknowledge received for domain:%s, queue:%s.", domain, queueOrTopic);
				}
				
				mq_threads_event_processor_release_acknowledge_waiting_evtitem("", pchn->replyQueue, queueMsg->id);
			}
			else{
				MQ_LOG_WARN("mq_pubsub_send_queue: failed in adding item to sending queue for domain:%s, queue:%s, priority: %d.", domain, queueOrTopic, priority);
			}
        }
        else
        {
            result = MQ_SYSTEM_ERROR_TRY_AGAIN;
        }
    }
    else
    {
        MQ_LOG_INFO("mq_pubsub_send_queue: adding item to sending queue (no ack) for domain:%s, queue:%s, message: %s, priority: %d.", domain, queueOrTopic, message, priority);
        char * popt = (mq_config_priority_supported() > 0) ? MQ_QUEUE_PSEND : MQ_QUEUE_SEND;
        result = mq_threads_send_add_entry(domain, queueMsg->id, popt, queueOrTopic, message, timeout, priority);
		if(result != MQ_OK) {
			MQ_LOG_WARN("mq_pubsub_send_queue: Failed in adding item to sending queue (no ack) for domain:%s, queue:%s, priority: %d.", domain, queueOrTopic, priority);
		}
    }
    
    msg_destroy(queueMsg);
    free(message);
    
    return result;
}

int mq_pubsub_send_topic(const char* domain, const char* topic, const char* payload, const int size, const long timeout) {
    int result = MQ_OK;

    MQMessage* topicMsg = mq_msg_create(MQ_MSG_TYPE_TOPIC, domain, topic, payload, size, NULL, 0);

    // to json string
    char* message = NULL;
    
    if(topicMsg)
    {
        message = msg_json_serialize(topicMsg);
    }

    if(message)
    {
        MQ_LOG_INFO("mq_pubsub_send_topic: adding item to sending topic for domain:%s, topic:%s, message: %s.", domain, topic, message);
        result = mq_threads_send_add_entry(domain, topicMsg->id, MQ_PUBLISH, topic, message, timeout, MQ_MSG_PRIORITY_DEFAULT);
		
		if(result != MQ_OK){
			MQ_LOG_INFO("mq_pubsub_send_topic: failed in adding item to sending topic for domain:%s, topic:%s.", domain, topic);
		}
    }
    
    // destroy topic msg
    if(topicMsg) msg_destroy(topicMsg);

    if(message) free(message);

    return result;
}

int mq_pubsub_acknowledge_queue(const char* replybuffer, const int replybuffersize, const int ackstatus, const long timeout)
{
    MQMessage* oneMsg = msg_json_parse(replybuffer);
    if(oneMsg)
    {
        MQ_LOG_INFO("mq_pubsub_acknowledge_queue: sending back acknowledge for domain:%s, replyqueue:%s, message: %s.", 
                        oneMsg->toDomain, oneMsg->queueOrTopic, replybuffer);
        char * popt = (mq_config_priority_supported() > 0) ? MQ_QUEUE_PSEND : MQ_QUEUE_SEND;
        char* packstring = (char*)replybuffer;
        if(ackstatus != 0){
			int iack = ackstatus % 256;
			snprintf(oneMsg->payload, MQ_ACKNOWLEDGE_PAYLOAD_SIZE, "%03d", iack);
            packstring = msg_json_serialize(oneMsg);
        }

        int result = mq_threads_send_add_entry(oneMsg->toDomain, oneMsg->id, popt, oneMsg->queueOrTopic, packstring, timeout, oneMsg->priority);
		if(result != MQ_OK) {
			MQ_LOG_WARN("mq_pubsub_acknowledge_queue: failed in sending back acknowledge for domain:%s, replyqueue:%s.", 
                        oneMsg->toDomain, oneMsg->queueOrTopic);
		}
        
        msg_destroy(oneMsg);

        if(packstring != replybuffer){
            free(packstring);
        }
    }
    else
    {
        MQ_LOG_INFO("mq_pubsub_acknowledge_queue: invalid acknowledge message.");
    }
    return MQ_OK;
}

int mq_pubsub_trigger_receive_admintopicqueue(const char* domain,  int isTQtype){
	int iret = MQ_OK;
	if(isTQtype == MQ_TQTYPE_QUEUE){
		iret = mq_pubsub_send_queue(domain, NAME_QUEUE_ADMIN, "0", 1, MQ_MSG_TIMEOUT_DEFAULT, NULL, MQ_MSG_TYPE_QUEUE_NORMAL, MQ_MSG_PRIORITY_DEFAULT);
		MQ_LOG_INFO("mq_pubsub_trigger_receive_admintopicqueue: admin queue is triggered for domain:%s.", domain);
	}
	else{
		iret = mq_pubsub_send_topic(domain, NAME_TOPIC_ADMIN, "0", 1, MQ_MSG_TIMEOUT_DEFAULT);
		MQ_LOG_INFO("mq_pubsub_trigger_receive_admintopicqueue: admin topic is triggered for domain:%s.", domain);
	}

	return iret;
}

MQMessage* mq_msg_create(int msgType, const char* domain, const char* topic, const char* payload, const int size, MQChannel * chnn, const int priority) 
{
    int isize = sizeof(MQMessage) + size + MQ_ALIGNMENT_BUFFER_SIZE;
    if(chnn)
    {
        isize += sizeof(MQChannel);
    }
    
    MQMessage* oneMsg = malloc(isize);
    memset(oneMsg, 0, isize);
    
    if(oneMsg) {
        oneMsg->totalSize = isize;
        oneMsg->payloadSize = size;
        oneMsg->messageType = msgType;
        oneMsg->priority = priority;
        memcpy(oneMsg->payload, payload, size + 1);
        memcpy(oneMsg->queueOrTopic, topic, MQ_TOPIC_SIZE);
        memcpy(oneMsg->toDomain, domain, MQ_DOMAIN_SIZE);
        memcpy(oneMsg->fromDomain, mq_get_local_domain(), MQ_DOMAIN_SIZE);
        oneMsg->id = mq_msg_id_generate();
        oneMsg->timestamp = time(NULL);
        oneMsg->ttl = MQ_MSG_TTL_DEFAULT;
        oneMsg->version = MQ_MSG_VERSION;
        if(chnn)
        {
            void* pd = (void*)oneMsg;
            pd = pd + sizeof(MQMessage) + size + 1;
            oneMsg->channel = pd;
            memcpy(oneMsg->channel, chnn, sizeof(MQChannel));
        }
    }
    
    return oneMsg;
}

void msg_destroy(MQMessage* msg) 
{
    free(msg);
}

char* msg_json_serialize(const MQMessage* msg) {
    char* strRet = NULL;
    JSON_Value *jroot = json_value_init_object();
    char strSeq[20];
    memset(strSeq, 0, 20);
    snprintf(strSeq, 20, "%09d", msg->id);
    json_object_set_string(json_object(jroot), "seq", strSeq);
    json_object_set_string(json_object(jroot), "payload", msg->payload);
    json_object_set_integer(json_object(jroot), "payloadSize", msg->payloadSize);
    json_object_set_integer(json_object(jroot), "id", msg->id);
    json_object_set_integer(json_object(jroot), "timestamp", msg->timestamp);
    json_object_set_integer(json_object(jroot), "ttl", msg->ttl);
    if(msg->priority) json_object_set_integer(json_object(jroot), "priority", msg->priority);
    json_object_set_integer(json_object(jroot), "version", msg->version);
    json_object_set_integer(json_object(jroot), "messageType", msg->messageType);
    json_object_set_string(json_object(jroot), "toDomain", msg->toDomain);
    json_object_set_string(json_object(jroot), "fromDomain", msg->fromDomain);
    json_object_set_string(json_object(jroot), "queueOrTopic", msg->queueOrTopic);
	if(msg->rawSize > 0){  		//if rawSize is the message size before zip, if message is not zipped, it will be zero;
		json_object_set_integer(json_object(jroot), "rawSize", msg->rawSize);
	}

    if(msg->channel)
    {
        JSON_Value* jchannel = json_value_init_object();
        json_object_set_value(json_object(jroot), "channel", jchannel);

        MQChannel* c = msg->channel;
        json_object_set_string(json_object(jchannel), "domain", c->domain);
        json_object_set_string(json_object(jchannel), "queName", c->queName);
        json_object_set_integer(json_object(jchannel), "priority", c->priority);
        json_object_set_integer(json_object(jchannel), "replyMode", c->replyMode);
        json_object_set_string(json_object(jchannel), "replyQueue", c->replyQueue);
		if(c->zipMode > 0){
			json_object_set_integer(json_object(jchannel), "zipMode", c->zipMode);
		}
    } else 
    {
        json_object_set_string(json_object(jroot), "channel", "null");
    }

    strRet = json_serialize_to_string(jroot);
    json_value_free(jroot);
    return strRet;
    
}

MQMessage* msg_json_parse(const char* jstring) {
    MQMessage* oneMsg = NULL;
    JSON_Value *jvroot = json_parse_string(jstring);
    if(jvroot)
    {
        JSON_Object* jroot = json_value_get_object(jvroot);
        MQChannel chnn;
        MQChannel* pchMsg = NULL;
        if(jroot)
        {
            const char* pstring = NULL;
            int imsgsize = sizeof(MQMessage);
            JSON_Object *jchnn = json_object_get_object(jroot, "channel");
            if(jchnn)
            {
                imsgsize += sizeof(MQChannel);
                pchMsg = &chnn;
                pstring = json_object_get_string(jchnn, "domain");
                if(pstring )strncpy(pchMsg->domain, pstring, MQ_DOMAIN_SIZE);

                pstring = json_object_get_string(jchnn, "queName");
                if(pstring )strncpy(pchMsg->queName, pstring, MQ_TOPIC_SIZE);

                pstring = json_object_get_string(jchnn, "replyQueue");
                if(pstring )strncpy(pchMsg->replyQueue, pstring, MQ_TOPIC_SIZE);
                
                pchMsg->priority = (int)json_object_get_integer(jchnn, "priority");
                pchMsg->replyMode = (int)json_object_get_integer(jchnn, "replyMode");
				pchMsg->zipMode = (int)json_object_get_integer(jchnn, "zipMode");
            }
            
            pstring = json_object_get_string(jroot, "payload");
            if(pstring)
            {
                imsgsize += strlen(pstring);
            }
            imsgsize += MQ_ALIGNMENT_BUFFER_SIZE;
            
            oneMsg = (MQMessage*)malloc(imsgsize);
            memset(oneMsg, 0, imsgsize);
            oneMsg->totalSize = imsgsize;
            
            if(pchMsg)
            {
                oneMsg->channel = (void*)oneMsg + sizeof(MQMessage) + strlen(pstring) + 1;
            }
            
            if(pstring) strncpy(oneMsg->payload, pstring, strlen(pstring)+1);
            
            pstring = json_object_get_string(jroot, "fromDomain");
            if(pstring )strncpy(oneMsg->fromDomain, pstring, MQ_DOMAIN_SIZE);
            
            pstring = json_object_get_string(jroot, "toDomain");
            if(pstring )strncpy(oneMsg->toDomain, pstring, MQ_DOMAIN_SIZE);

            pstring = json_object_get_string(jroot, "queueOrTopic");
            if(pstring )strncpy(oneMsg->queueOrTopic, pstring, MQ_TOPIC_SIZE);
            
            oneMsg->id = (long)json_object_get_integer(jroot, "id");
            oneMsg->timestamp = (long)json_object_get_integer(jroot, "timestamp");
            oneMsg->payloadSize = (int)json_object_get_integer(jroot, "payloadSize");
            oneMsg->ttl = (int)json_object_get_integer(jroot, "ttl");
            oneMsg->version = (int)json_object_get_integer(jroot, "version");
            oneMsg->messageType = (int)json_object_get_integer(jroot, "messageType");
            oneMsg->priority=0;
            if(json_object_get_object(jroot, "priority") != NULL)
                oneMsg->priority = (int)json_object_get_integer(jroot, "priority");

			oneMsg->rawSize=(int)json_object_get_integer(jroot, "rawSize");
            
            if(pchMsg)
            {
                memcpy(oneMsg->channel, pchMsg, sizeof(MQChannel));
            }
        }
        
        json_value_free(jvroot);
    }
    return oneMsg;
}

char* msg_json_get_ack_message(const MQMessage* msg)
{
    char* strRet = "";
    if( (msg) && (msg->channel) )
    {
        int iacksize = sizeof(MQMessage) + MQ_ACKNOWLEDGE_PAYLOAD_SIZE;
        MQMessage* ack_msg = (MQMessage*)malloc(iacksize);
        memcpy(ack_msg, msg, iacksize);
        ack_msg->payloadSize = 1;
        ack_msg->messageType = MQ_MSG_TYPE_QUEUE_ACK;
        ack_msg->timestamp = time(NULL);
        ack_msg->channel = NULL;
		memset(ack_msg->payload, '0', MQ_ACKNOWLEDGE_PAYLOAD_SIZE - 1);
        ack_msg->payload[MQ_ACKNOWLEDGE_PAYLOAD_SIZE - 1] = 0;
        memcpy(ack_msg->queueOrTopic, msg->channel->replyQueue, MQ_TOPIC_SIZE);
        memcpy(ack_msg->fromDomain, mq_get_local_domain(), MQ_DOMAIN_SIZE);
        memcpy(ack_msg->toDomain, msg->fromDomain, MQ_DOMAIN_SIZE);
        
        strRet = msg_json_serialize(ack_msg);
        msg_destroy(ack_msg);
    }
    return strRet;
}
