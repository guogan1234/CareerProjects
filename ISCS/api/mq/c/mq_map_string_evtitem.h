#ifndef MQ_MAP_STRING_EVTITEM_H
#define MQ_MAP_STRING_EVTITEM_H
#include <c_map.h>
#include "mq_buffer_list.h"
#include "mq_map_utilities.h"

typedef struct mq_map_string_itemevent_t mq_map_string_itemevent_t;

mq_map_string_itemevent_t* mq_map_string_itemevent_create();

void* mq_map_string_itemevent_add_item(mq_map_string_itemevent_t* maptm, const char* key, void* value);

void* mq_map_string_itemevent_get_item(mq_map_string_itemevent_t* maptm, const char* key);

void mq_map_string_itemevent_remove_item(mq_map_string_itemevent_t* maptm, const char* key);

void mq_map_string_itemevent_release(mq_map_string_itemevent_t* maptm);

#endif