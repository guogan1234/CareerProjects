/*
 * @Copyright Reserved by XXXX.
 * Double link list.
 * Create by KanWenDi, 2018.06.13.
 * Histories:
 */
#include "rtdb_cli_list.h"
#include <stdlib.h>
#include <string.h>

struct rtdb_cli_list_node
{
    struct rtdb_cli_list_node *prev;
    struct rtdb_cli_list_node *next;
    void *value;
};

struct rtdb_cli_list
{
	unsigned long length;
	rtdb_cli_list_data_free free;
    rtdb_cli_list_data_compare compare;
    struct rtdb_cli_list_node *head;
    struct rtdb_cli_list_node *tail;
    struct rtdb_cli_list_node *iter;
};

rtdb_cli_list* rtdb_cli_list_create(rtdb_cli_list_data_free free, rtdb_cli_list_data_compare compare)
{
    rtdb_cli_list *l;
    
    l = malloc(sizeof(*l));
    if(l)
    {
        l->head = l->tail = l->iter = NULL;
        l->free = free;
        l->compare = compare;
        l->length = 0;
    }
    
    return l;
}

void rtdb_cli_list_free(rtdb_cli_list* l)
{
    if(!l) return;
    rtdb_cli_list_clear(l);
    free(l);
}

void rtdb_cli_list_clear(rtdb_cli_list* l)
{
    unsigned long len;
    struct rtdb_cli_list_node *current, *next;
    
    if (!l) return;

    current = l->head;
    len = l->length;
    while (len--)
    {
        next = current->next;
        if (l->free) l->free(current->value);
        free(current);
        current = next;
    }
    l->head = l->tail = l->iter = NULL;
    l->length = 0;
}

rtdb_cli_list* rtdb_cli_list_push_head(rtdb_cli_list *l, void *value)
{
    struct rtdb_cli_list_node *node;
    
    if (!l || !value) return NULL;
    
    node = malloc(sizeof(*node));
    if(!node) return NULL;
    
    node->value = value;
    if(l->length == 0)
    {
        l->head = l->tail = node;
        node->prev = node->next = NULL;
    }
    else
    {
        node->prev = NULL;
        node->next = l->head;
        l->head->prev = node;
        l->head = node;
    }
    
    l->length += 1;
    
    return l;
}

rtdb_cli_list* rtdb_cli_list_push_tail(rtdb_cli_list *l, void *value)
{
    struct rtdb_cli_list_node *node;
    
    if (!l || !value) return NULL;
    
    node = malloc(sizeof(*node));
    if(!node) return NULL;
    
    node->value = value;
    if(l->length == 0)
    {
        l->head = l->tail = node;
        node->prev = node->next = NULL;
    }
    else
    {
        node->prev = l->tail;
        node->next = NULL;
        l->tail->next = node;
        l->tail = node;        
    }
    
    l->length += 1;
    
    return l;
}

void rtdb_cli_list_remove(rtdb_cli_list *l, void *value)
{
    struct rtdb_cli_list_node *node, *target;
    
    if (!l || !value) return;
    
    target = NULL;
    node = l->head;
    while (node)
    {
		if (l->compare)
		{
			if (l->compare(node->value, value))
			{
				target = node;
				break;
			}
		}
		else
		{
			if (node->value == value)
			{
				target = node;
				break;
			}
		}
		
		node = node->next;
	}
	
	if (!target) return;
	
    if (target->prev)
    {
		target->prev->next = target->next;
	}
    else
    {
		l->head = target->next;
	}
	
    if (target->next)
    {
		target->next->prev = target->prev;
	}
    else
    {
		l->tail = target->prev;
	}
        
    if (l->free) l->free(target->value);
    
    free(target);
    l->length -= 1;
}

void* rtdb_cli_list_get_first(rtdb_cli_list *l)
{
	if (!l || !l->head) return NULL;
	
	l->iter = l->head;
	return l->iter->value;
}

void* rtdb_cli_list_get_next(rtdb_cli_list *l)
{
	if (!l || !l->iter) return NULL;
	
	l->iter = l->iter->next;
	return l->iter ? l->iter->value : NULL;
}

unsigned long rtdb_cli_list_get_size(rtdb_cli_list *l)
{
	return l ? l->length : 0;
}
