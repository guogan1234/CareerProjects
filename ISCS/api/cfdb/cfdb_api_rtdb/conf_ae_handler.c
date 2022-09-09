#include "conf_ae_handler.h"
#include "conf_net.h"
#include "conf_priv_data.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void conf_cleanup(aeEventLoop *el, int fd)
{
    aeDeleteFileEvent(el, fd, AE_READABLE);
    conf_management_remove_conn_byfd(fd, 1);
}

void conf_aehandler_read_response(aeEventLoop *el, int fd, void *privdata, int mask)
{
    char *msg;
    int32_t iRet, msgLen, readLen;
    conf_response_message *cmsg;
    conf_conn *cc = (conf_conn*)privdata;

    ((void)el);
    ((void)mask);

    iRet = conf_net_read_int32(&cc->net, &msgLen);
    if (iRet == -1)
    {
        conf_cleanup(el, fd);
        return;
    }
    if ((msgLen <= 0 || ((uint32_t)msgLen) > INT32_MAX))
    {
        conf_cleanup(el, fd);
        return;
    }

    readLen = msgLen - 4;
    if (!(msg = malloc(readLen+1)))
    {
        conf_cleanup(el, fd);
        return;
    }

    memset(msg, 0, readLen+1);
    iRet = conf_net_read_bytes(&cc->net, msg, &readLen);
    if (iRet == -1 || ((msgLen <= 0 || ((uint32_t)msgLen) > INT32_MAX)))
    {
        conf_cleanup(el, fd);
        free(msg);
        return;
    }

    cmsg = conf_create_response_message(fd, msg, readLen);
    if (!cmsg)
    {
        free(msg);
        return;
    }

    iRet = conf_management_push_message(cmsg);
    if (iRet == -1)
    {
        free(msg);
        return;
    }

    // weakup parse thread
    px_thread_lock(g_confMgr.parseThread);
    px_thread_resume(g_confMgr.parseThread);
    px_thread_unlock(g_confMgr.parseThread);
}
