#include "sysconf.h"
#include "sysconf_macros.h"
#include "sysconf_data_private.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

static void usage();
static void trace_domain(sysconf_domain *domain, char *sp);
static void trace_node(sysconf_node *node, char *sp);
static void trace_process(sysconf_process *process, char *sp);
static void trace_mq_client(sysconf_mq_client *c, char *sp);
static void trace_pm_client(sysconf_pm_client *c, char *sp);
static void trace_tsdb_client(sysconf_tsdb_client *c, char *sp);
static void trace_rtdb_client(sysconf_rtdb_client *c, char *sp);
static void trace_cfdb_client(sysconf_cfdb_client *c, char *sp);
static void trace_system_manager(sysconf_system_manager *sm, char *sp);
static void trace_pair(sysconf_pair *pair, int32_t idx, char *sp);
static void trace_nic(sysconf_nic *nic, int32_t idx, char *sp);
static void trace_instance(sysconf_instance *instance, int32_t idx, char *sp);

int main(int argc, char* argv[])
{
    int32_t code = 0;
    char cmd[64] = {0};

    // 初始化系统配置运行环境
    code = sysconf_init();
    if (code == SYSCONF_EC_OK)
    {
        printf("sysconf initialized ok\n");
    }
    else
    {
        printf("sysconf initialized error(%d)\n", code);
        return 0;
    }

    // 打印帮助信息
    usage();

    while (1)
    {
        scanf("%s", cmd);

        if ((strcasecmp(cmd, "quit") == 0) || strcasecmp(cmd, "exit") == 0)
        {
            printf("quit\n");
            break;
        }
        else if (strcasecmp(cmd, "getnd") == 0)
        {
            sysconf_domain *domain = NULL;

            code = sysconf_get_native_domain(&domain);
            if (code == SYSCONF_EC_OK)
            {
                trace_domain(domain, "  ");
            }
            else
            {
                printf("get native domain error(%d)\n", code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getnn") == 0)
        {
            sysconf_node *node = NULL;

            code = sysconf_get_native_node(&node);
            if (code == SYSCONF_EC_OK)
            {
                trace_node(node, "  ");
            }
            else
            {
                printf("get native node error(%d)\n", code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getsm") == 0)
        {
            sysconf_system_manager *sm = NULL;

            code = sysconf_get_system_manager(&sm);
            if (code == SYSCONF_EC_OK)
            {
                trace_system_manager(sm, "  ");
            }
            else
            {
                printf("get system manager error(%d)\n", code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getmqcli") == 0)
        {
            sysconf_mq_client *mq = NULL;

            code = sysconf_get_mq_client(&mq);
            if (code == SYSCONF_EC_OK)
            {
                trace_mq_client(mq, "  ");
            }
            else
            {
                printf("get mq client error(%d)\n", code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getrtdbcli") == 0)
        {
            sysconf_rtdb_client *rtdb = NULL;

            code = sysconf_get_rtdb_client(&rtdb);
            if (code == SYSCONF_EC_OK)
            {
                trace_rtdb_client(rtdb, "");
            }
            else
            {
                printf("get rtdb client error(%d)\n", code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "gettsdbcli") == 0)
        {
            sysconf_tsdb_client *tsdb = NULL;

            code = sysconf_get_tsdb_client(&tsdb);
            if (code == SYSCONF_EC_OK)
            {
                trace_tsdb_client(tsdb, "");
            }
            else
            {
                printf("get tsdb client error(%d)\n", code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getcfdbcli") == 0)
        {
            sysconf_cfdb_client *cfdb = NULL;

            code = sysconf_get_cfdb_client(&cfdb);
            if (code == SYSCONF_EC_OK)
            {
                trace_cfdb_client(cfdb, "");
            }
            else
            {
                printf("get cfdb client error(%d)\n", code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getpmcli") == 0)
        {
            sysconf_pm_client *pm = NULL;

            code = sysconf_get_pm_client(&pm);
            if (code == SYSCONF_EC_OK)
            {
                trace_pm_client(pm, "");
            }
            else
            {
                printf("get process manager error(%d)\n", code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getd") == 0)
        {
            int32_t id = 0;
            sysconf_domain *domain = NULL;

            scanf("%d", &id);
            code = sysconf_get_domain(id, &domain);
            if (code == SYSCONF_EC_OK)
            {
                trace_domain(domain, "");
            }
            else
            {
                printf("get domain(%d) error(%d)\n", id, code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getn") == 0)
        {
            int32_t id = 0;
            sysconf_node *node = NULL;

            scanf("%d", &id);
            code = sysconf_get_node(id, &node);
            if (code == SYSCONF_EC_OK)
            {
                trace_node(node, "");
            }
            else
            {
                printf("get node(%d) error(%d)\n", id, code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "getp") == 0)
        {
            int32_t type = 0;
            sysconf_process *process = NULL;

            scanf("%d", &type);
            code = sysconf_get_process(type, &process);
            if (code == SYSCONF_EC_OK)
            {
                trace_process(process, "");
            }
            else
            {
                printf("get process(%d) error(%d)\n", type, code);
            }
            printf("sysconf>");
        }
        else if (strcasecmp(cmd, "help") == 0)
        {
            usage();
        }
        else
        {
            usage();
            printf("\nunsuported command(%s)\n", cmd);
            printf("sysconf>");
        }
    }

    sysconf_uninit();

    return 0;
}

static void usage()
{
    printf("system configure test.\n");
    printf("all supported commands:\n");
    printf("quit/exit                   quit program.\n");
    printf("help                        show usage.\n");
    printf("getnd                       get native domain\n");
    printf("getnn                       get native node\n");
    printf("getsm                       get system manager\n");
    printf("getmqcli                    get mq client\n");
    printf("getrtdbcli                  get rtdb client\n");
    printf("gettsdbcli                  get tsdb client\n");
    printf("getcfdbcli                  get cfdb client\n");
    printf("getpmcli                    get process manager client\n");
    printf("getd  id                    get domain\n");
    printf("getn  id                    get node\n");
    printf("getp  id                    get process\n");
    printf("sysconf>");
}

static void trace_domain(sysconf_domain *domain, char *sp)
{
    int32_t i = 0;
    char space[32] = {0}, space2[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);
    snprintf(space2, sizeof(space2), "%s%s%s", sp, sp, sp);

    if (!domain)
    {
        printf("%s", sp);
        printf("domain null\n");
        return;
    }

    printf("%s", sp);
    printf("domain:\n");

    printf("%s", space);
    printf("id=%d\n", domain->id);

    printf("%s", space);
    printf("type=%d\n", domain->type);

    printf("%s", space);
    printf("name=%s\n", domain->name);

    printf("%s", space);
    printf("name_ch=%s\n", domain->nameCH);

    printf("%s", space);
    printf("node_num=%d\n", domain->nodeNum);

    for (int32_t i = 0; i < domain->nodeNum; i++)
    {
        printf("%s", space);
        printf("node[%d]:\n", i + 1);

        printf("%s", space2);
        printf("id=%d\n", domain->nodes[i]->id);

        printf("%s", space2);
        printf("name=%s\n", domain->nodes[i]->name);
    }
}

static void trace_node(sysconf_node *node, char *sp)
{
    int32_t i = 0;
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    if (!node)
    {
        printf("%s", sp);
        printf("node null\n");
        return;
    }

    printf("%s", sp);
    printf("node:\n");

    printf("%s", space);
    printf("id=%d\n", node->id);

    printf("%s", space);
    printf("type=%d\n", node->type);

    printf("%s", space);
    printf("status=%d\n", node->status);

    printf("%s", space);
    printf("netStatus=%d\n", node->netStatus);

    printf("%s", space);
    printf("stationId=%d\n", node->stationId);

    printf("%s", space);
    printf("domainId=%d\n", node->domain->id);

    printf("%s", space);
    printf("name=%s\n", node->name);

    printf("%s", space);
    printf("name_ch=%s\n", node->nameCH);

    printf("%s", space);
    printf("dev=%s\n", node->dev);

    printf("%s", space);
    printf("os=%s\n", node->os);

    printf("%s", space);
    printf("nic_num=%d\n", node->nicNum);

    for (i = 0; i < node->nicNum; i++)
    {
        trace_nic(&node->nics[i], i + 1, space);
    }

    printf("%s", space);
    printf("instance_num=%d\n", node->instanceNum);

    for (i = 0; i < node->instanceNum; i++)
    {
        trace_instance(&node->instances[i], i + 1, space);
    }
}

static void trace_process(sysconf_process *process, char *sp)
{
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    if (!process)
    {
        printf("%s", sp);
        printf("process empty\n");
        return;
    }

    printf("%s", sp);
    printf("process:\n");

    printf("%s", space);
    printf("type=%d\n", process->type);

    printf("%s", space);
    printf("start_type=%d\n", process->startType);

    printf("%s", space);
    printf("auto_run=%d\n", process->autoRun);

    printf("%s", space);
    printf("cycle_start=%d\n", process->cycleStart);

    printf("%s", space);
    printf("cycle_period=%d\n", process->cyclePeriod);

    printf("%s", space);
    printf("offline_run=%d\n", process->offlineRun);

    printf("%s", space);
    printf("duty_run=%d\n", process->dutyRun);

    printf("%s", space);
    printf("standby_run=%d\n", process->standbyRun);

    printf("%s", space);
    printf("run_order=%d\n", process->runOrder);

    printf("%s", space);
    printf("report_type=%d\n", process->reportType);

    printf("%s", space);
    printf("run_style=%d\n", process->runStyle);

    printf("%s", space);
    printf("alias=%s\n", process->alias);

    printf("%s", space);
    printf("command=%s\n", process->command);

    printf("%s", space);
    printf("file_path=%s\n", process->filePath);

    printf("%s", space);
    printf("description=%s\n", process->description);
}

static void trace_mq_client(sysconf_mq_client *c, char *sp)
{
    int32_t i = 0;
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    if (!c)
    {
        printf("%s", sp);
        printf("mq client empty\n");
        return;
    }

    printf("%s", sp);
    printf("mq client:\n");

    printf("%s", space);
    printf("port=%d\n", c->port);

    printf("%s", space);
    printf("pool_size=%d\n", c->poolSize);

    printf("%s", space);
    printf("timeout=%d\n", c->timeout);

    printf("%s", space);
    printf("heartbeat_interval=%d\n", c->heartbeatInterval);

    printf("%s", space);
    printf("schema=%s\n", c->schema);

    printf("%s", space);
    printf("user_name=%s\n", c->userName);

    printf("%s", space);
    printf("password=%s\n", c->password);

    printf("%s", space);
    printf("pair_num=%d\n", c->pairNum);

    for (i = 0; i < c->pairNum; i++)
    {
        trace_pair(&c->pairs[i], i + 1, space);
    }

    printf("%s", space);
    printf("nic_num=%d\n", c->nicNum);

    for (i = 0; i < c->nicNum; i++)
    {
        trace_nic(c->nics[i], i + 1, space);
    }
}

static void trace_pm_client(sysconf_pm_client *c, char *sp)
{
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    if (!c)
    {
        printf("%s", sp);
        printf("process manager client empty\n");
        return;
    }

    printf("%s", sp);
    printf("process manager client:\n");

    printf("%s", space);
    printf("port=%d\n", c->port);

    printf("%s", space);
    printf("switch_period=%d\n", c->switchPeriod);

    printf("%s", space);
    printf("logon_period=%d\n", c->logonPeriod);

    printf("%s", space);
    printf("max_clients=%d\n", c->maxClients);

    printf("%s", space);
    printf("max_idle_time=%d\n", c->maxIdleTime);

    printf("%s", space);
    printf("tcp_backlog=%d\n", c->tcpBacklog);

    printf("%s", space);
    printf("tcp_keep_alive=%d\n", c->tcpKeepAlive);

    printf("%s", space);
    printf("back_ground=%d\n", c->backGround);

    printf("%s", space);
    printf("local_domain=%d\n", c->localDomain);

    printf("%s", space);
    printf("nic_num=%d\n", c->nicNum);

    for (int32_t i = 0; i < c->nicNum; i++)
    {
        trace_nic(c->nics[i], i + 1, space);
    }
}

static void trace_tsdb_client(sysconf_tsdb_client *c, char *sp)
{
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    if (!c)
    {
        printf("%s", sp);
        printf("tsdb client empty\n");
        return;
    }

    printf("%s", sp);
    printf("tsdb client:\n");

    printf("%s", space);
    printf("port=%d\n", c->port);

    printf("%s", space);
    printf("timeout=%d\n", c->timeout);

    printf("%s", space);
    printf("heartbeat_interval=%d\n", c->heartbeatInterval);

    printf("%s", space);
    printf("schema=%s\n", c->schema);

    printf("%s", space);
    printf("user_name=%s\n", c->userName);

    printf("%s", space);
    printf("password=%s\n", c->password);

    printf("%s", space);
    printf("database=%s\n", c->database);

    printf("%s", space);
    printf("nic_num=%d\n", c->nicNum);

    for (int32_t i = 0; i < c->nicNum; i++)
    {
        trace_nic(c->nics[i], i + 1, space);
    }
}

static void trace_rtdb_client(sysconf_rtdb_client *c, char *sp)
{
    int32_t i = 0;
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    if (!c)
    {
        printf("%s", sp);
        printf("rtdb client empty\n");
        return;
    }

    printf("%s", sp);
    printf("rtdb client:\n");

    printf("%s", space);
    printf("rtdb_port=%d\n", c->rtdbPort);

    printf("%s", space);
    printf("safe_port=%d\n", c->safePort);

    printf("%s", space);
    printf("user_name=%s\n", c->userName);

    printf("%s", space);
    printf("password=%s\n", c->password);

    printf("%s", space);
    printf("pair_num=%d\n", c->pairNum);

    for (i = 0; i < c->pairNum; i++)
    {
        trace_pair(&c->pairs[i], i + 1, space);
    }

    printf("%s", space);
    printf("nic_num=%d\n", c->nicNum);

    for (i = 0; i < c->nicNum; i++)
    {
        trace_nic(c->nics[i], i + 1, space);
    }
}

static void trace_cfdb_client(sysconf_cfdb_client *c, char *sp)
{
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    if (!c)
    {
        printf("%s", sp);
        printf("cfdb client empty\n");
        return;
    }

    printf("%s", sp);
    printf("cfdb client:\n");

    printf("%s", space);
    printf("conf_port=%d\n", c->confPort);

    printf("%s", space);
    printf("sync_port=%d\n", c->syncPort);

    printf("%s", space);
    printf("url_path=%s\n", c->urlPath);

    printf("%s", space);
    printf("nic_num=%d\n", c->nicNum);

    for (int32_t i = 0; i < c->nicNum; i++)
    {
        trace_nic(c->nics[i], i + 1, space);
    }
}

static void trace_system_manager(sysconf_system_manager *sm, char *sp)
{
    char space[32] = {0}, space2[32] = {0};
    int32_t i = 0;

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);
    snprintf(space2, sizeof(space2), "%s%s%s", sp, sp, sp);

    if (!sm)
    {
        printf("%s", sp);
        printf("system manager empty\n");
        return;
    }

    printf("%s", sp);
    printf("system manager:\n");

    printf("%s", space);
    printf("domain_num=%d\n", sm->domainNum);

    printf("%s", space);
    printf("domains:\n");

    for (i = 0; i < sm->domainNum; i++)
    {
        trace_domain(&sm->domains[i], space2);
    }

    printf("%s", space);
    printf("node_num=%d\n", sm->nodeNum);

    printf("%s", space);
    printf("nodes:\n");

    for (i = 0; i < sm->nodeNum; i++)
    {
        trace_node(&sm->nodes[i], space2);
    }

    printf("%s", space);
    printf("process_num=%d\n", sm->processNum);

    printf("%s", space);
    printf("prcoesses:\n");

    for (i = 0; i < sm->processNum; i++)
    {
        trace_process(&sm->processes[i], space2);
    }
}

static void trace_pair(sysconf_pair *pair, int32_t idx, char *sp)
{
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    printf("%s", sp);
    printf("pair[%d]:\n", idx);

    printf("%s", space);
    printf("key=%s\n", pair->key);

    printf("%s", space);
    printf("value=%s\n", pair->value);
}

static void trace_nic(sysconf_nic *nic, int32_t idx, char *sp)
{
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    printf("%s", sp);
    printf("nic[%d]:\n", idx);

    printf("%s", space);
    printf("id=%d\n", nic->id);

    printf("%s", space);
    printf("node_id=%d\n", nic->nodeId);

    printf("%s", space);
    printf("name=%s\n", nic->name);

    printf("%s", space);
    printf("netmask=%s\n", nic->netmask);

    printf("%s", space);
    printf("address=%s\n", nic->address);
}

static void trace_instance(sysconf_instance *instance, int32_t idx, char *sp)
{
    char space[32] = {0};

    sp = sp ? sp : "";
    snprintf(space, sizeof(space), "%s%s", sp, sp);

    printf("%s", sp);
    printf("instance[%d]:\n", idx);

    printf("%s", space);
    printf("id=%d\n", instance->id);

    printf("%s", space);
    printf("param=%s\n", instance->param);

    printf("%s", space);
    printf("type=%d\n", instance->process->type);

    printf("%s", space);
    printf("alias=%s\n", instance->process->alias);
}
