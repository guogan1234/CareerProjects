#ifndef CMN_LIST_H
#define CMN_LIST_H
#ifdef __cplusplus
extern "C" {
#endif

/* Directions for iterators */
#define CMN_LIST_HEAD 0
#define CMN_LIST_TAIL 1

typedef void*(*cmn_list_cb_dup)(void *val);
typedef void(*cmn_list_cb_free)(void *val);
typedef int(*cmn_list_cb_match)(void *val, void *key);

typedef struct cmn_list cmn_list;
typedef struct cmn_list_node cmn_list_node;
typedef struct cmn_list_iterator cmn_list_iterator;

cmn_list* cmn_list_create(cmn_list_cb_dup dup, cmn_list_cb_free free, cmn_list_cb_match match);
void cmn_list_release(cmn_list *l);
cmn_list* cmn_list_add_head(cmn_list *l, void *value);
cmn_list* cmn_list_add_tail(cmn_list *l, void *value);
cmn_list* cmn_list_add(cmn_list *l, cmn_list_node *ln, void *value, int after);
void cmn_list_remove(cmn_list *l, cmn_list_node *ln);
cmn_list_iterator* cmn_list_get_iterator(cmn_list *l, int direction);
void cmn_list_release_iterator(cmn_list_iterator *iter);
cmn_list_node* cmn_list_next(cmn_list_iterator *iter);
cmn_list_node* cmn_list_find(cmn_list *l, void *key);
cmn_list_node* cmn_list_get_at(cmn_list *l, long index);
long cmn_list_get_size(cmn_list *l);
cmn_list_node* cmn_list_get_frist(cmn_list *l);
cmn_list_node* cmn_list_get_last(cmn_list *l);
void* cmn_list_node_get_previous(cmn_list_node *ln);
void* cmn_list_node_get_next(cmn_list_node *ln);
void* cmn_list_node_get_value(cmn_list_node *ln);
cmn_list* cmn_list_dup(cmn_list *l);

#ifdef __cplusplus
}
#endif
#endif
