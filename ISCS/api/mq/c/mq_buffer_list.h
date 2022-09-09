#ifndef MQ_BUFFER_LIST_H
#define MQ_BUFFER_LIST_H

typedef struct mq_buffer_list_t mq_buffer_list_t;

mq_buffer_list_t* mq_buffer_list_create();

void mq_buffer_list_add_item(mq_buffer_list_t* lst, value_type item);

value_type mq_buffer_list_get_item(mq_buffer_list_t* lst, int blocked);

value_type mq_buffer_list_get_item_timeout(mq_buffer_list_t* lst, int timeout);

void mq_buffer_list_release(mq_buffer_list_t* lst);

void mq_buffer_list_unblock(mq_buffer_list_t* lst);

int mq_buffer_list_is_empty(mq_buffer_list_t* lst);

int mq_buffer_list_size(mq_buffer_list_t* lst);


typedef int (*mq_buffer_item_checkor)(void* par1, void* par2);
value_type mq_buffer_list_get_item_by_condition(mq_buffer_list_t* lst, mq_buffer_item_checkor pProc, void* inData);
value_type  mq_buffer_list_remove_item_by_condition(mq_buffer_list_t* lst, mq_buffer_item_checkor pProc, void* inData);

#endif