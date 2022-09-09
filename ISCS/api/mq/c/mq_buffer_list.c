/*
 @Copyright Reserved by .
 mq.
 Define buffer list with protection of critic resource.
 Create by 
 Histories:

 */
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <c_list.h>
#include "cmn_lock.h"
#include "cmn_event.h"
#include "mq_buffer_list.h"
#include <stdio.h>
#include "mq_cmn_def.h"

struct mq_buffer_list_t
{
    cmn_lock* 	lock;
	cmn_event* 	event;
    cmn_event* 	event_max_buffer;
    int         max_buffer_item;
	c_list		lst_data;
};

mq_buffer_list_t* mq_buffer_list_create(int maxitm)
{
	mq_buffer_list_t *lst = malloc(sizeof(*lst));
    if (lst)
    {
        lst->lock = cmn_lock_create();
        lst->event = cmn_event_create();
		c_list_create(&lst->lst_data, NULL);
        lst->event_max_buffer = NULL;
        lst->max_buffer_item = 0;
        
        if(maxitm>0)
        {
            lst->max_buffer_item = maxitm;
            lst->event_max_buffer = cmn_event_create();
            cmn_event_trigger(lst->event_max_buffer);
        }
        
        MQ_LOG_DEBUG("mq_buffer_list_create: max_buffer(%d) Done.", lst->max_buffer_item);
    }
    else
    {
        MQ_LOG_ERR("mq_buffer_list_create: failed in allocating memory for buffer list.");
    }

    return lst;
}

void mq_buffer_list_add_item(mq_buffer_list_t* lst, value_type item)
{
	if (lst)
    {
        if(lst->event_max_buffer)
        {
            cmn_event_wait(lst->event_max_buffer);
        }
        
		cmn_lock_lock(lst->lock);
        
		c_list_push_back(&lst->lst_data, item);
		cmn_event_trigger(lst->event);
        
        if((lst->event_max_buffer != NULL) && (c_list_size(&lst->lst_data) >= lst->max_buffer_item) )
        {
            cmn_event_reset(lst->event_max_buffer);
        }

		cmn_lock_unlock(lst->lock);
        
        MQ_LOG_DEBUG("mq_buffer_list_add_item: one item added into the list.");
        
    }
    else
    {
        MQ_LOG_ERR("mq_buffer_list_add_item: invalid buffer list provided.");
    }
}

value_type mq_buffer_list_get_item(mq_buffer_list_t* lst, int blocked)
{
    value_type retVal = NULL;
    if (lst)
    {
        if(blocked !=0)
        {
            cmn_event_wait(lst->event);
        }
        
        cmn_lock_lock(lst->lock);
        
        if(c_list_empty(&lst->lst_data) == false)
		{
			retVal = c_list_front(&lst->lst_data);
			c_list_pop_front(&lst->lst_data) ;
			
            if(c_list_empty(&lst->lst_data) == true)
			{
                cmn_event_reset(lst->event);
				//cmn_event_trigger(lst->event);
			}
			
		}
        
        if((lst->event_max_buffer != NULL) && (c_list_size(&lst->lst_data) < lst->max_buffer_item) )
        {
            cmn_event_trigger(lst->event_max_buffer);
        }

		cmn_lock_unlock(lst->lock);
        
        MQ_LOG_DEBUG("mq_buffer_list_get_item: %s.", (retVal==NULL ? "empty" : "one item"));
    }
    else
    {
        MQ_LOG_ERR("mq_buffer_list_add_item: invalid buffer list provided.");
    }
    
	return retVal;
}

value_type mq_buffer_list_get_item_timeout(mq_buffer_list_t* lst, int timeout)
{
    value_type retVal = NULL;
    if (lst)
    {
        int evttriggered = cmn_event_timedwait(lst->event, timeout);
        
        if(evttriggered)
        {
            cmn_lock_lock(lst->lock);
        
            if(c_list_empty(&lst->lst_data) == false)
            {
                retVal = c_list_front(&lst->lst_data);
                c_list_pop_front(&lst->lst_data);
                
                if(c_list_empty(&lst->lst_data) == true)
                {
                    cmn_event_reset(lst->event);
                    //cmn_event_trigger(lst->event);
                }
                
            }

            if((lst->event_max_buffer != NULL) && (c_list_size(&lst->lst_data) < lst->max_buffer_item) )
            {
                cmn_event_trigger(lst->event_max_buffer);
            }
        
            cmn_lock_unlock(lst->lock);
            MQ_LOG_DEBUG("mq_buffer_list_get_item_timeout within time: %s.", (retVal==NULL ? "empty" : "one item"));
        }
        else
        {
            MQ_LOG_DEBUG("mq_buffer_list_get_item_timeout timeout.");
        }
    }
    else
    {
        MQ_LOG_ERR("mq_buffer_list_get_item_timeout: invalid buffer list provided.");
    }
	return retVal;
}

void mq_buffer_list_release(mq_buffer_list_t* lst)
{
    if (lst)
    {
		cmn_event_release(lst->event);
        cmn_lock_release(lst->lock);
        
        if(lst->event_max_buffer){
            cmn_event_release(lst->event_max_buffer);
        }
		
		//To remove and release all list item
		while(c_list_empty(&lst->lst_data) == false)
		{
			free(c_list_front(&lst->lst_data));
			c_list_pop_front(&lst->lst_data) ;
		}
		
        c_list_destroy(&lst->lst_data);
		free(lst);
        MQ_LOG_DEBUG("mq_buffer_list_release done.");
    }
}

void mq_buffer_list_unblock(mq_buffer_list_t* lst)
{
	if (lst)
    {
		cmn_lock_lock(lst->lock);
		cmn_event_trigger(lst->event);
		cmn_lock_unlock(lst->lock);
        
        MQ_LOG_DEBUG("mq_buffer_list_unblock: mq buffer list event is triggered.");
    }
    else
    {
        MQ_LOG_ERR("mq_buffer_list_add_item: invalid buffer list provided.");
    }	
}

int mq_buffer_list_is_empty(mq_buffer_list_t* lst)
{
    int iRet = 0;
    if(lst)
    {
        cmn_lock_lock(lst->lock);
            
        if(c_list_empty(&lst->lst_data) == true)
            iRet = 1;
        
        cmn_lock_unlock(lst->lock);
    }
    return iRet;
}

int mq_buffer_list_size(mq_buffer_list_t* lst)
{
    int iRet = 0;
    if(lst)
    {
        cmn_lock_lock(lst->lock);
        iRet = c_list_size(&lst->lst_data);
        cmn_lock_unlock(lst->lock);
    }
    //MQ_LOG_DEBUG("mq_buffer_list_size, size(%d).", iRet);
    return iRet;
}

value_type mq_buffer_list_get_item_by_condition(mq_buffer_list_t* lst, mq_buffer_item_checkor pProc, void* inData)
{
    value_type retV = NULL;
    if(lst)
    {
        cmn_lock_lock(lst->lock);
            
        if(pProc)
        {
            c_iterator iter;
            c_iterator st_ite = c_list_begin(&lst->lst_data);
            c_iterator ed_ite = c_list_end(&lst->lst_data);
            for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
            {
                if(pProc(ITER_REF(iter), inData) > 0)
                {
                    retV = ITER_REF(iter);
                    break;
                }
            }
        }
        
        cmn_lock_unlock(lst->lock);
        
        MQ_LOG_DEBUG("mq_buffer_list_get_item_by_condition: %s.", (retV==NULL ? "empty" : "one item"));
    }
    else
    {
        MQ_LOG_ERR("mq_buffer_list_get_item_by_condition: invalid buffer list provided.");
    }
    
    return retV;
}

value_type  mq_buffer_list_remove_item_by_condition(mq_buffer_list_t* lst, mq_buffer_item_checkor pProc, void* inData)
{
    value_type retV = NULL;
    if(lst)
    {
        cmn_lock_lock(lst->lock);
            
        if(pProc)
        {
            c_iterator iter;
            c_iterator st_ite = c_list_begin(&lst->lst_data);
            c_iterator ed_ite = c_list_end(&lst->lst_data);
            for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
            {
                if(pProc(ITER_REF(iter), inData) > 0)
                {
                    retV = ITER_REF(iter);
                    c_list_erase(&lst->lst_data, iter);
                    break;
                }
            }
        }
        
        cmn_lock_unlock(lst->lock);
        
        MQ_LOG_DEBUG("mq_buffer_list_remove_item_by_condition: %s.", (retV==NULL ? "empty" : "one item"));
    }
    else
    {
        MQ_LOG_ERR("mq_buffer_list_get_item_by_condition: invalid buffer list provided.");
    }
    return retV;
}