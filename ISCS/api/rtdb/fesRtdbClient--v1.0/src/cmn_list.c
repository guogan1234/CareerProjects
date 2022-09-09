/*
 @Copyright Reserved by XXXX.
 Double link list.
 Create by KanWenDi, 2018.04.03.
 Histories:

 */
#include <stdlib.h>
#include <string.h>
#include "cmn_list.h"

struct cmn_list_node
{
    struct cmn_list_node *prev;
    struct cmn_list_node *next;
    void *value;
};

struct cmn_list_iterator
{
    struct cmn_list_node *next;
    int direction;
};

struct cmn_list
{
    struct cmn_list_node *head;
    struct cmn_list_node *tail;
    cmn_list_cb_dup dup;
    cmn_list_cb_free free;
    cmn_list_cb_match match;
    long length;
};

cmn_list* cmn_list_create(cmn_list_cb_dup dup, cmn_list_cb_free free, cmn_list_cb_match match)
{
    struct cmn_list *list = NULL;
    list = malloc(sizeof(struct cmn_list));
    if(list)
    {
        list->head = list->tail = NULL;
        list->dup = dup;
        list->free = free;
        list->match = match;
        list->length = 0;
    }
    return list;
}

void cmn_list_empty(cmn_list *list)
{
    unsigned long len;
    cmn_list_node *current, *next;

    current = list->head;
    len = list->length;
    while(len--) {
        next = current->next;
        if (list->free) list->free(current->value);
        free(current);
        current = next;
    }
    list->head = list->tail = NULL;
    list->length = 0;
}

void cmn_list_release(cmn_list *l)
{
    if(l)
    {
        cmn_list_empty(l);
        free(l);
        l = NULL;
    }
}

cmn_list* cmn_list_add_head(cmn_list *l, void *value)
{
    cmn_list_node *node = NULL;
    node = malloc(sizeof(cmn_list_node));
    if(NULL == node)
        return NULL;
    node->value = value;
    if(0 == l->length)
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

cmn_list* cmn_list_add_tail(cmn_list *l, void *value)
{
    cmn_list_node *node = NULL;
    node = malloc(sizeof(cmn_list_node));
    if(NULL == node)
        return NULL;
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

cmn_list* cmn_list_add(cmn_list *l, cmn_list_node *ln, void *value, int after)
{
    cmn_list_node *node;
    node = malloc(sizeof(cmn_list_node));
    if(NULL == node)
        return NULL;
    node->value = value;
    if(after)
    {
        node->prev = ln;
        node->next = ln->next;
        if(l->tail == ln)
        {
            l->tail = node;
        }        
    }
    else
    {
        node->next = ln;
        node->prev = ln->prev;
        if (l->head == ln) 
        {
            l->head = node;
        }
    }
    
    if(node->prev != NULL)
    {
        node->prev->next = node;
    }
    if(node->next != NULL)
    {
        node->next->prev = node;
    }
    l->length += 1;
    return NULL;
}

void cmn_list_remove(cmn_list *l, cmn_list_node *ln)
{
    if (ln->prev)
        ln->prev->next = ln->next;
    else
        l->head = ln->next;
    if (ln->next)
        ln->next->prev = ln->prev;
    else
        l->tail = ln->prev;
    if (l->free) l->free(ln->value);
    free(ln);
    l->length -= 1;
}

cmn_list_iterator* cmn_list_get_iterator(cmn_list *l, int direction)
{
    cmn_list_iterator *iter;
    iter = malloc(sizeof(cmn_list_iterator));
    if(NULL == iter)
        return NULL;
    if (direction == CMN_LIST_HEAD)
        iter->next = l->head;
    else
        iter->next = l->tail;
    iter->direction = direction;
    return iter;
}

void cmn_list_release_iterator(cmn_list_iterator *iter)
{
    if(iter)
    {
        free(iter);
        iter = NULL;
    }
}

cmn_list_node* cmn_list_next(cmn_list_iterator *iter)
{
    cmn_list_node *current = iter->next;
    if(current != NULL)
    {
        if(iter->direction == CMN_LIST_HEAD)
            iter->next = current->next;
        else
            iter->next = current->prev;
    }    
    return current;
}

void cmn_list_rewind(cmn_list *l, cmn_list_iterator *li) 
{
    li->next = l->head;
    li->direction = CMN_LIST_HEAD;
}

void cmn_list_rewind_tail(cmn_list *l, cmn_list_iterator *li) 
{
    li->next = l->tail;
    li->direction = CMN_LIST_TAIL;
}

cmn_list_node* cmn_list_find(cmn_list *l, void *key)
{
    cmn_list_iterator iter;
    cmn_list_node *node;

    cmn_list_rewind(l, &iter);
    while((node = cmn_list_next(&iter)) != NULL) 
    {
        if (l->match) 
        {
            if (l->match(node->value, key)) 
            {
                return node;
            }
        } 
        else 
        {
            if (key == node->value) 
            {
                return node;
            }
        }
    }
    return NULL;
}

cmn_list_node* cmn_list_get_at(cmn_list *l, long index)
{
    cmn_list_node *node=NULL;

    if (index < 0) 
    {
        index = (-index)-1;
        node = l->tail;
        while(index-- && node) 
            node = node->prev;
    } 
    else 
    {
        node = l->head;
        while(index-- && node) 
            node = node->next;
    }
    return node;
}

long cmn_list_get_size(cmn_list *l)
{
    return l->length;
}

cmn_list_node* cmn_list_get_frist(cmn_list *l)
{
    return l->head;
}

cmn_list_node* cmn_list_get_last(cmn_list *l)
{
    return l->tail;
}

void* cmn_list_node_get_previous(cmn_list_node *ln)
{
    return ln->prev;
}

void* cmn_list_node_get_next(cmn_list_node *ln)
{
    return ln->next;
}

void* cmn_list_node_get_value(cmn_list_node *ln)
{
    return ln->value;
}

cmn_list* cmn_list_dup(cmn_list *l)
{
    cmn_list *copy;
    cmn_list_iterator iter;
    cmn_list_node *node;
    
    copy = cmn_list_create(NULL,NULL,NULL);
    if (copy == NULL)
        return NULL;
    copy->dup = l->dup;
    copy->free = l->free;
    copy->match = l->match;
    cmn_list_rewind(l, &iter);
    while((node = cmn_list_next(&iter)) != NULL) 
    {
        void *value;
        if (copy->dup) 
        {
            value = copy->dup(node->value);
            if (value == NULL) 
            {
                cmn_list_release(copy);
                return NULL;
            }
        } 
        else
        {
            value = node->value;
        }
        
        if (cmn_list_add_tail(copy, value) == NULL) 
        {
            cmn_list_release(copy);
            return NULL;
        }
    }
    return copy;
}
