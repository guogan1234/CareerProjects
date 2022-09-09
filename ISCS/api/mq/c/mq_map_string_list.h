#ifndef MQ_MAP_STRING_LIST_H
#define MQ_MAP_STRING_LIST_H
#include <c_map.h>
#include "mq_buffer_list.h"
#include "mq_map_utilities.h"

typedef struct mq_map_string_list_t mq_map_string_list_t;

mq_map_string_list_t* mq_map_string_list_create();

void* mq_map_string_list_add_item_entry(mq_map_string_list_t* maptm, const char* key, void* value, int maxbuffsize);

void* mq_map_string_list_get_item(mq_map_string_list_t* maptm, const char* key);

void mq_map_string_list_remove_item(mq_map_string_list_t* maptm, const char* key);

void mq_map_string_list_release(mq_map_string_list_t* maptm);

#endif
