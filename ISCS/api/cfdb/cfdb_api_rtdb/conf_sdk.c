#include "conf_sdk.h"
#include "conf_priv_data.h"
#include <stdlib.h>

int conf_init(conf_load_notifier *load_notifier, conf_sync_notifier *sync_notifier)
{
    if (conf_management_init() == -1)
    {
        return -1;
    }

    g_confMgr.loadNotifier = load_notifier;
    g_confMgr.syncNotifier = sync_notifier;

    return conf_management_load_globalconf();
}

void conf_startup()
{
    conf_management_startup();
}

void conf_shutdown()
{
    conf_management_shutdown();
}

void conf_uninit()
{
    conf_management_destroy();
}

int conf_async_load_table(const char *tblname,int domain_id_flag)
{
    return conf_management_request_table(tblname,domain_id_flag);
}
