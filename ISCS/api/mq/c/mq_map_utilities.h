#ifndef MQ_MAP_UTILITIES_H
#define MQ_MAP_UTILITIES_H
#include "cmn_lock.h"
#include "cmn_event.h"
#include "mq_priv.h"

#define MQ_MAX_SIZE_EVENT_POOL          20

typedef struct mq_string_key_t     mq_string_key_t;
typedef struct mq_events_pool mq_events_pool;
typedef struct event_item_entry event_item_entry;

struct mq_string_key_t
{
    int                 hashCode;
    int                 keytype;    //0 using strKey, 1 using ptrKey
    union{
        char            strKey[0];
        char*           ptrKey;
    };
};


struct event_item_entry
{
    void*       pData;
    int         iStatus;
    cmn_lock*   plock;
    cmn_event*  pevt;
};

int _stringkey_comparer(void * x, void * y);

void mq_map_string_list_reform_temp_key(mq_string_key_t* pkey, const char* inp);

mq_string_key_t* mq_map_string_list_create_key(const char* inp);

int mq_get_hash_from_string(const char* inp);


//Event Pools
mq_events_pool* mq_events_pool_create();

event_item_entry* mq_events_pool_get_event_item(mq_events_pool* pool);

void mq_events_pool_release_event_item(mq_events_pool* pool, event_item_entry* pentry, bool freeData);

void mq_events_pool_release(mq_events_pool* pool);

#endif