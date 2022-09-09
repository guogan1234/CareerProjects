/**
 * @file rtdbcli_handlemap.h
 * @author
 * @date 
 * @version 
 * @note 句柄处理。
 **/
#ifndef RTDBCLI_HANDLEMAP_H
#define RTDBCLI_HANDLEMAP_H

#include <stdint.h>
#include <pthread.h>

#define INIT_SLOTS 16


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct handle_entry {
	int32_t                 handleid;
	int                     ref_count;
	void *                  ud;
} _handle_entry, *_phandle_entry;

typedef struct handle_map {
	int32_t                 lasthandleid;
	pthread_mutex_t         mutex;
	int                     capacity;
	int                     count;
	struct handle_entry*    handles;
} _handle_map, *_phandle_map;


_handle_map* handle_map_init(int initSize);
void handle_map_release(_handle_map * hm);

int32_t handle_map_add_entry(_handle_map* hm, void * entry);

void* handle_map_get_entry(_handle_map* hm, int32_t hid);

void* handle_map_release_entry(_handle_map* hm, int32_t hid);

int handle_map_list(_handle_map* hm, int count, int32_t* res);


#ifdef __cplusplus
}
#endif
#endif
