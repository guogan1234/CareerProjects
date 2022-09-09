#ifndef CONF_AEHANDLER_H
#define CONF_AEHANDLER_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "ae.h"

void conf_aehandler_read_response(aeEventLoop *el, int fd, void *privdata, int mask);

#ifdef __cplusplus
}
#endif
#endif
