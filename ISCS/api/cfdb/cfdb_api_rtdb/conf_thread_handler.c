#include "conf_thread_handler.h"
#include "conf_message.h"
#include "conf_priv_data.h"
#include <pxthread/px_thread.h>

void* conf_threadhandler_recive_message(void *arg)
{
    px_thread *pxthread = (px_thread*)arg;

    while (px_thread_get_status(pxthread) == PX_THREAD_RUNNING)
    {
        if (!g_confMgr.el)
        {
            break;
        }

        aeMain(g_confMgr.el);
    }

    return (void*)0;
}

void* conf_threadhandler_parse_message(void *arg)
{
    conf_table *table;
    conf_response_message *cmsg;
    px_thread *pxthread = (px_thread*)arg;

    while (px_thread_get_status(pxthread) == PX_THREAD_RUNNING)
    {
        cmsg = conf_management_pop_message();
        if (!cmsg)
        {
            px_thread_lock(pxthread);
            px_thread_suspend(pxthread);
            px_thread_unlock(pxthread);
            cmsg = conf_management_pop_message();
            if (!cmsg) continue;
        }

        table = conf_message_parse_response(cmsg->msg, cmsg->msgLen);
        if (!table)
        {
            conf_management_remove_conn_byfd(cmsg->fd, 1);
            conf_free_response_message(cmsg);
            continue;
        }

        if (table->status < 0)
        {
            conf_management_remove_conn_byfd(cmsg->fd, 1);
        }
		else if (table->recordNum == 0)
        {
            conf_management_remove_conn_byfd(cmsg->fd, 0);
        }
        conf_free_response_message(cmsg);

        if (conf_management_push_table(table) == -1)
        {
            conf_free_table(table);
            continue;
        }

        // resume notify thread
        px_thread_lock(g_confMgr.notifyThread);
        px_thread_resume(g_confMgr.notifyThread);
        px_thread_unlock(g_confMgr.notifyThread);
    }

    return (void*)0;
}

void* conf_threadhandler_notify_message(void *arg)
{
    conf_table *table;
    px_thread *pxthread = (px_thread*)arg;

    while (px_thread_get_status(pxthread) == PX_THREAD_RUNNING)
    {
        table = conf_management_pop_table();
        if (!table)
        {
            px_thread_lock(pxthread);
            px_thread_suspend(pxthread);
            px_thread_unlock(pxthread);
            table = conf_management_pop_table();
            if (!table) continue;
        }

        if (g_confMgr.loadNotifier)
        {
            g_confMgr.loadNotifier((const conf_table*)table);
        }
        conf_free_table(table);
    }

    return (void*)0;
}
