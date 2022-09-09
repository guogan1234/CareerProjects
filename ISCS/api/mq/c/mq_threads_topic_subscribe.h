#ifndef MQ_THREADS_SUBSCRIBE_H
#define MQ_THREADS_SUBSCRIBE_H

#include <pthread.h>
#include <c_vector.h>
#include <c_list.h>
#include <pthread.h>
#include "cmn_lock.h"
#include "mq_buffer_list.h"


typedef struct threads_topic_subscribe_param_t     threads_topic_subscribe_param_t;
typedef struct data_topic_subscribe_entry          data_topic_subscribe_entry;


struct threads_topic_subscribe_param_t
{
    int                 index;
    pthread_t*          pthrd;
    MQSafeSession*      session;
    mq_buffer_list_t*   list_buffer_message;
    //c_list              list_subscribed_topics;   
    //cmn_lock*           lock_param;
};

struct data_topic_subscribe_entry
{
    long    timeout;
    char    opt[MQ_OPTION_SIZE];
    char    domain[MQ_DOMAIN_SIZE];
    char    queueOrTopic[MQ_TOPIC_SIZE];
    MessageListener    messageHandler;
};


void mq_threads_topic_subscribe_init();

int mq_threads_topic_subscribe_start();

void* mq_threads_topic_subscribe_run(void* ptr);

void mq_threads_topic_subscribe_stop();

void mq_threads_topic_subscribe_join();

int mq_threads_topic_subscribe_add_entry(const char* opt, const char * domain, const char* topic, const MessageListener listener);

#endif
