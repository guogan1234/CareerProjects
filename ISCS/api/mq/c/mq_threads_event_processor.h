#ifndef MQ_THREADS_EVENT_PROCESSOR_H
#define MQ_THREADS_EVENT_PROCESSOR_H

typedef struct data_event_processor_entry          data_event_processor_entry;

struct data_event_processor_entry
{
    MessageListener    messageHandler;
    char    domain[MQ_DOMAIN_SIZE];
    char    queueOrTopic[MQ_TOPIC_SIZE];
    int     messageSize;
    int     replySize;
    char    messageData[0]; 
};

void mq_threads_event_processor_init(int numThrd);

void mq_threads_event_processor_stop();

void mq_threads_event_processor_add_task(const char* domain, const char* topic, const char* strMsg, const int size, const MessageListener listener, int* outChannelPriority);

void* mq_threads_event_processor_get_response_buffers(const char* domain, const char* topic);

void* mq_threads_event_processor_create_response_buffers(const char* domain, const char* topic);

void* mq_threads_event_processor_get_acknowledge_waiting_evtitem(const char* domain, const char* topic, long idMsg);

void* mq_threads_event_processor_create_acknowledge_waiting_evtitem(const char* domain, const char* topic, long idMsg);

void mq_threads_event_processor_release_acknowledge_waiting_evtitem(const char* domain, const char* topic, long idMsg);

#endif