/**
 * @file sysconf_json_loader.c
 * @author
 * @date 
 * @version 
 * @note 系统全局配置接口。本接口文件定义系统全局配置用到的JSON配置文件加载接口实现。
 *
 **/
#include "sysconf_json_loader.h"
#include "sysconf_data_private.h"
#include "sysconf_macros.h"
#include "sysconf_malloc.h"
#include <log.h>
#include <errno.h>
#include <parson.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#ifdef _WIN32
#include <winsock2.h>
#endif

static int32_t sysconf_parse_domains(JSON_Object *jso);
static int32_t sysconf_parse_nodes(JSON_Object *jso);
static int32_t sysconf_parse_processes(JSON_Object *jso);
static int32_t sysconf_parse_nics(sysconf_node *node, JSON_Object *jso);
static int32_t sysconf_parse_instances(sysconf_node *node, JSON_Object *jso);
static int32_t sysconf_parse_mqcli(JSON_Object *jso);
static int32_t sysconf_parse_rtdbcli(JSON_Object *jso);
static int32_t sysconf_parse_tsdbcli(JSON_Object *jso);
static int32_t sysconf_parse_cfdbcli(JSON_Object *jso);
static int32_t sysconf_parse_pmcli(JSON_Object *jso);
static int32_t sysconf_parse_pairs(JSON_Array *jsa, sysconf_pair *pairs, int32_t size);
static int32_t sysconf_parse_string(JSON_Object *jso, const char *key, char *val, int32_t vallen);
static int32_t sysconf_is_instance_exist(const sysconf_node *node, int32_t typenum, va_list types);
static sysconf_nic** sysconf_make_native_nics(int32_t *size, int32_t typenum, ...);
static int32_t sysconf_get_hostname(char *hostname, int32_t len);

int32_t sysconf_load_deployconf_json(const char *file)
{
    int32_t handle = -1, ec = SYSCONF_EC_OK;
    JSON_Value *jsv = NULL;
    JSON_Object *jso = NULL;
    sysconf_conf *sf = NULL;

    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;

    // 检查输入的文件名，名称为空返回错误
    if (!file)
    {
        log_trace_err(handle, "sysconf_load_deployconf_json, file null");
        return SYSCONF_EC_NULLPTR;
    }

    log_trace_debug(handle, "sysconf_load_deployconf_json, load file(%s)", file);

    // 加载解析JSON格式配置文件，失败错误返回
    jsv = json_parse_file(file);
    if (!jsv)
    {
        log_trace_err(handle, "sysconf_load_deployconf_json, file(%s) error", file);
        return SYSCONF_EC_CFGERR;
    }

    jso = json_value_get_object(jsv);
    if (!jso)
    {
        log_trace_err(handle, "sysconf_load_deployconf_json, config error");
        json_value_free(jsv);
        return SYSCONF_EC_CFGERR;
    }

    // 首先解析进程配置，然后解析域配置，最后解析节点配置
    // 解析进程配置，失败错误返回
    ec = sysconf_parse_processes(jso);
    if (ec != SYSCONF_EC_OK)
    {
        log_trace_err(handle, "sysconf_load_deployconf_json, parse processes error");
        json_value_free(jsv);
        return ec;
    }
    log_trace_debug(handle, "sysconf_load_deployconf_json, parse processes ok");

    // 解析域配置，失败错误返回
    ec = sysconf_parse_domains(jso);
    if (ec != SYSCONF_EC_OK)
    {
        log_trace_err(handle, "sysconf_load_deployconf_json, parse domains error");
        json_value_free(jsv);
        return ec;
    }
    log_trace_debug(handle, "sysconf_load_deployconf_json, parse domains ok");

    // 解析节点配置，失败错误返回
    ec = sysconf_parse_nodes(jso);
    if (ec != SYSCONF_EC_OK)
    {
        log_trace_err(handle, "sysconf_load_deployconf_json, parse nodes error");
        json_value_free(jsv);
        return ec;
    }
    log_trace_debug(handle, "sysconf_load_deployconf_json, parse nodes ok");

    json_value_free(jsv);
    log_trace_debug(handle, "sysconf_load_deployconf_json, ok");

    return SYSCONF_EC_OK;
}

int32_t sysconf_load_runconf_json(const char *file)
{
    int32_t handle = -1, ec = SYSCONF_EC_OK;
    JSON_Value *jsv = NULL;
    JSON_Object *jso = NULL;
    sysconf_conf *sf = NULL;

    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;

    // 检查输入的文件名，为空时错误返回
    if (!file)
    {
        log_trace_err(handle, "sysconf_load_runconf_json, file null");
        return SYSCONF_EC_NULLPTR;
    }

    log_trace_debug(handle, "sysconf_load_runconf_json, load file(%s)", file);

    // 加载并解析JSON配置文件
    jsv = json_parse_file(file);
    if (!jsv)
    {
        log_trace_err(handle, "sysconf_load_runconf_json, file(%s) error", file);
        return SYSCONF_EC_CFGERR;
    }

    jso = json_value_get_object(jsv);
    if (!jso)
    {
        log_trace_err(handle, "sysconf_load_runconf_json, config error");
        json_value_free(jsv);
        return SYSCONF_EC_CFGERR;
    }

    // 解析MQ客户端配置
    ec = sysconf_parse_mqcli(jso);
    if (ec != SYSCONF_EC_OK)
    {
        log_trace_err(handle, "sysconf_load_runconf_json, parse mq client error");
        json_value_free(jsv);
        return ec;
    }
    log_trace_debug(handle, "sysconf_load_runconf_json, parse mq client ok");

    // 解析RTDB API配置
    ec = sysconf_parse_rtdbcli(jso);
    if (ec != SYSCONF_EC_OK)
    {
        log_trace_err(handle, "sysconf_load_runconf_json, parse rtdb client error");
        json_value_free(jsv);
        return ec;
    }
    log_trace_debug(handle, "sysconf_load_runconf_json, parse rtdb client ok");

    // 解析TSDB客户端配置
    ec = sysconf_parse_tsdbcli(jso);
    if (ec != SYSCONF_EC_OK)
    {
        log_trace_err(handle, "sysconf_load_runconf_json, parse tsdb client error");
        json_value_free(jsv);
        return ec;
    }
    log_trace_debug(handle, "sysconf_load_runconf_json, parse tsdb client ok");

    // 解析配置数据库服务客户端配置
    ec = sysconf_parse_cfdbcli(jso);
    if (ec != SYSCONF_EC_OK)
    {
        log_trace_err(handle, "sysconf_load_runconf_json, parse cfdb client error");
        json_value_free(jsv);
        return ec;
    }
    log_trace_debug(handle, "sysconf_load_runconf_json, parse cfdb client ok");

    // 解析进程管理客户端配置
    ec = sysconf_parse_pmcli(jso);
    if (ec != SYSCONF_EC_OK)
    {
        log_trace_err(handle, "sysconf_load_runconf_json, parse process manager client error");
        json_value_free(jsv);
        return ec;
    }
    log_trace_debug(handle, "sysconf_load_runconf_json, parse process manager client ok");

    json_value_free(jsv);
    log_trace_debug(handle, "sysconf_load_runconf_json, ok");

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_domains(JSON_Object *jso)
{
    int32_t i = 0, size = 0, handle = -1, ec = SYSCONF_EC_OK;
    JSON_Array *jsa = NULL;
    JSON_Object *jsoe = NULL;
    sysconf_domain *domains = NULL;
    sysconf_conf *sf = NULL;

    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;

    // 获取域配置列表，失败错误返回
    jsa = json_object_get_array(jso, "DOMAINS");
    if (!jsa)
    {
        log_trace_err(handle, "sysconf_parse_domains, no domain");
        return SYSCONF_EC_NOEXIST;
    }

    size = json_array_get_count(jsa);
    if (size == 0)
    {
        log_trace_err(handle, "sysconf_parse_domains, domain empty");
        return SYSCONF_EC_NOEXIST;
    }

    // 为系统管理对象的域申请内存
    domains = sysconf_malloc(sizeof(sysconf_domain)*size);
    if (!domains)
    {
        log_trace_err(handle, "sysconf_parse_domains, oom");
        return SYSCONF_EC_OOM;
    }

    memset(domains, 0, sizeof(sysconf_domain)*size);
    sf->sm.domainNum = size;
    sf->sm.domains = domains;

    // 遍历域JSON数据，生成域对象
    // 域节点配置内容：ID、TYPE、NAME、NAME_CH
    for (i = 0; i < size; i++)
    {
        jsoe = json_array_get_object(jsa, i);
        domains[i].id = json_object_get_integer(jsoe, "ID");
        domains[i].type = json_object_get_integer(jsoe, "TYPE");

        ec = sysconf_parse_string(jsoe, "NAME", domains[i].name, sizeof(domains[i].name));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_string(jsoe, "NAME_CH", domains[i].nameCH, sizeof(domains[i].nameCH));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }
    }

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_nodes(JSON_Object *jso)
{
    int32_t domainId = 0, handle = -1, ec = SYSCONF_EC_OK;
    int32_t i = 0, j = 0, idx = 0, size = 0;
    char hostname[SYSCONF_NAME_MAX_LEN] = {0};
    JSON_Array *jsa = NULL;
    JSON_Object *jsoe = NULL;
    sysconf_node *nodes = NULL;
    sysconf_system_manager *sm = NULL;
    sysconf_conf *sf = NULL;

    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;
    sm = &sf->sm;

    // 获取节点JSON数组
    jsa = json_object_get_array(jso, "NODES");
    if (!jsa)
    {
        log_trace_err(handle, "sysconf_parse_nodes, no node");
        return SYSCONF_EC_NOEXIST;
    }

    size = json_array_get_count(jsa);
    if (size == 0)
    {
        log_trace_err(handle, "sysconf_parse_nodes, node empty");
        return SYSCONF_EC_NOEXIST;
    }

    // 为系统管理的节点申请内存
    nodes = sysconf_malloc(sizeof(sysconf_node)*size);
    if (!nodes)
    {
        log_trace_err(handle, "sysconf_parse_nodes, oom");
        return SYSCONF_EC_OOM;
    }

    memset(nodes, 0, sizeof(sysconf_node)*size);
    sm->nodeNum = size;
    sm->nodes = nodes;
    

    // 遍历节点JSON数组，生成节点对象
    // 节点对象配置项:ID、TYPE、STATUS、NET_STATUS、STATION_ID、
    // DOMAIN_ID、NAME、NAME_CH、DEV、OS、NICS、INSTANCES
    for (i = 0; i < size; i++)
    {

        jsoe = json_array_get_object(jsa, i);
        
        nodes[i].id = json_object_get_integer(jsoe, "ID");
        nodes[i].type = json_object_get_integer(jsoe, "TYPE");
        nodes[i].status = json_object_get_integer(jsoe, "STATUS");
        nodes[i].netStatus = json_object_get_integer(jsoe, "NET_STATUS");
        nodes[i].stationId = json_object_get_integer(jsoe, "STATION_ID");

        ec = sysconf_parse_string(jsoe, "NAME", nodes[i].name, sizeof(nodes[i].name));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_string(jsoe, "NAME_CH", nodes[i].nameCH, sizeof(nodes[i].nameCH));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_string(jsoe, "DEV", nodes[i].dev, sizeof(nodes[i].dev));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_string(jsoe, "OS", nodes[i].os, sizeof(nodes[i].os));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_nics(&nodes[i], jsoe);
        if (ec != SYSCONF_EC_OK)
        {
            log_trace_err(handle, "sysconf_parse_nodes, parse nics error");
            return ec;
        }

        ec = sysconf_parse_instances(&nodes[i], jsoe);
        if (ec != SYSCONF_EC_OK)
        {
            log_trace_err(handle, "sysconf_parse_nodes, parse instance error");
            return ec;
        }

        // 关联域和节点
        // 把域关联到节点，给域做节点计数
        domainId = json_object_get_integer(jsoe, "DOMAIN_ID");
        for (j = 0; j < sm->domainNum; j++)
        {
            if (sm->domains[j].id == domainId)
            {
                nodes[i].domain = &sm->domains[j];
                sm->domains[j].nodeNum++;
                break;
            }
        }

        // 检查节点是否是本地域的节点
        if (hostname[0] == '\0')
        {
            if (sysconf_get_hostname(hostname, sizeof(hostname)) != SYSCONF_EC_OK)
            {
                log_trace_err(handle, "sysconf_parse_nodes, get hostname error");
            }
            else
            {
                log_trace_err(handle, "sysconf_parse_nodes, get hostname ok");
            }
        }

        if ((hostname[0] != '\0') && (strcasecmp(hostname, nodes[i].name) == 0))
        {
            if (sm->native)
            {
                log_trace_warn(handle, "sysconf_parse_nodes, multi-native node error");
            }
            sm->native = &nodes[i];
        }
    }

    // 遍历域数组，给把节点关联到域
    for (i = 0; i < sm->domainNum; i++)
    {
        sm->domains[i].nodes = sysconf_malloc(sizeof(sysconf_node*)*sm->domains[i].nodeNum);
        if (!sm->domains[i].nodes)
        {
            log_trace_err(handle, "sysconf_parse_nodes, oom");
            return SYSCONF_EC_OOM;
        }
        memset(sm->domains[i].nodes, 0, sizeof(sysconf_node*)*sm->domains[i].nodeNum);

        idx = 0;
        for (j = 0; j < sm->nodeNum; j++)
        {
            if (sm->nodes[j].domain == (&sm->domains[i]))
            {
               sm->domains[i].nodes[idx++] = &sf->sm.nodes[j];
            }
        }
    }

    // 检查是否有本地域，如果没有则配置错误
    if (!sm->native)
    {
        log_trace_err(handle, "sysconf_parse_nodes, no native node is set");
        return SYSCONF_EC_NONATIVE;
    }

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_processes(JSON_Object *jso)
{
    int32_t i = 0, size = 0, handle = -1, ec = SYSCONF_EC_OK;
    JSON_Array *jsa = NULL;
    JSON_Object *jsoe = NULL;
    sysconf_system_manager *sm = NULL;
    sysconf_conf *sf = NULL;

    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;
    sm = &sf->sm;

    // 获取进程配置列表，失败错误返回
    jsa = json_object_get_array(jso, "PROCESSES");
    if (!jsa)
    {
        log_trace_err(handle, "sysconf_parse_processes, no process");
        return SYSCONF_EC_NOEXIST;
    }

    size = json_array_get_count(jsa);
    if (size == 0)
    {
        log_trace_err(handle, "sysconf_parse_processes, process empty ");
        return SYSCONF_EC_NOEXIST;
    }

    // 为系统管理对象的进程申请内存
    sm->processes = sysconf_malloc(sizeof(sysconf_process)*size);
    if (!sm->processes)
    {
        log_trace_err(handle, "sysconf_parse_processes, oom");
        return SYSCONF_EC_OOM;
    }

    sm->processNum = size;
    memset(sm->processes, 0, sizeof(sysconf_process)*size);

    // 遍历进程JSON数据，生成进程对象
    // 进程节点配置内容：TYPE、START_TYPE、CYCLE_START、CYCLE_PERIOD、RUN_ORDER
    // RUN_STYLE、AUTORUN、DUTY_RUN、OFFLINE_RUN、STANDBY_RUN、REPORT_TYPE、
    // ALIAS、COMMAND、FILE_PATH、DESCRIPTION、
    for (i = 0; i < size; i++)
    {
        jsoe = json_array_get_object(jsa, i);
        sm->processes[i].type = json_object_get_integer(jsoe, "TYPE");
        sm->processes[i].startType = json_object_get_integer(jsoe, "START_TYPE");
        sm->processes[i].cycleStart = json_object_get_integer(jsoe, "CYCLE_START");
        sm->processes[i].cyclePeriod = json_object_get_integer(jsoe, "CYCLE_PERIOD");
        sm->processes[i].runOrder = json_object_get_integer(jsoe, "RUN_ORDER");
        sm->processes[i].runStyle = json_object_get_integer(jsoe, "RUN_STYLE");
        sm->processes[i].autoRun = json_object_get_integer(jsoe, "AUTORUN");
        sm->processes[i].dutyRun = json_object_get_integer(jsoe, "DUTY_RUN");
        sm->processes[i].offlineRun = json_object_get_integer(jsoe, "OFFLINE_RUN");
        sm->processes[i].standbyRun = json_object_get_integer(jsoe, "STANDBY_RUN");
        sm->processes[i].reportType = json_object_get_integer(jsoe, "REPORT_TYPE");

        ec = sysconf_parse_string(jsoe, "ALIAS",
            sm->processes[i].alias, sizeof(sm->processes[i].alias));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_string(jsoe, "COMMAND",
            sm->processes[i].command, sizeof(sm->processes[i].command));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_string(jsoe, "FILE_PATH",
            sm->processes[i].filePath, sizeof(sm->processes[i].filePath));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_string(jsoe, "DESCRIPTION",
            sm->processes[i].description, sizeof(sm->processes[i].description));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }
    }

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_nics(sysconf_node *node, JSON_Object *jso)
{
    int32_t i = 0, size = 0, handle = -1, ec = SYSCONF_EC_OK;
    JSON_Array *jsa = NULL;
    JSON_Object *jsoe = NULL;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;

    // 从节点JSON对象取网卡数组，不存在错误返回
    jsa = json_object_get_array(jso, "NICS");
    if (!jsa)
    {
        log_trace_err(handle, "sysconf_parse_nics, no nic");
        return SYSCONF_EC_NOEXIST;
    }

    size = json_array_get_count(jsa);
    if (size == 0)
    {
        log_trace_err(handle, "sysconf_parse_nics, nic empty");
        return SYSCONF_EC_NOEXIST;
    }

    // 为节点的网卡申请内存，失败错误返回
    node->nics = sysconf_malloc(sizeof(sysconf_nic)*size);
    if (!node->nics)
    {
        log_trace_err(handle, "sysconf_parse_nics, oom");
        return SYSCONF_EC_OOM;
    }

    node->nicNum = size;
    memset(node->nics, 0, sizeof(sysconf_nic)*size);

    // 遍历网卡JSON数组，生成网卡对象
    // 网卡配置项：ID、NAME、NETMASK、ADDRESS
    for (i = 0; i < size; i++)
    {
        node->nics[i].nodeId = node->id;
        jsoe = json_array_get_object(jsa, i);

        node->nics[i].id = json_object_get_integer(jsoe, "ID");

        ec = sysconf_parse_string(jsoe, "NAME", node->nics[i].name, sizeof(node->nics[i].name));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        ec = sysconf_parse_string(jsoe, "NETMASK",
            node->nics[i].netmask, sizeof(node->nics[i].netmask));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }

        // IP地址不能不存在，否则视为错误
        ec = sysconf_parse_string(jsoe, "ADDRESS",
            node->nics[i].address, sizeof(node->nics[i].address));
        if (ec != SYSCONF_EC_OK)
        {
            return ec;
        }
    }

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_instances(sysconf_node *node, JSON_Object *jso)
{
    int32_t i = 0, j = 0, type = 0, found = 0, handle = -1;
    int32_t size = 0, ec = SYSCONF_EC_OK;
    JSON_Array *jsa = NULL;
    JSON_Object *jsoe = NULL;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;

    // 从节点JSON对象取进程实例，不存在错误返回
    jsa = json_object_get_array(jso, "INSTANCES");
    if (!jsa)
    {
        log_trace_err(handle, "sysconf_parse_instances, no instance");
        return SYSCONF_EC_NOEXIST;
    }

    size = json_array_get_count(jsa);
    if (size == 0)
    {
        log_trace_err(handle, "sysconf_parse_instances, instance empty");
        return SYSCONF_EC_NOEXIST;
    }

    // 为节点的进程实例申请内存，失败错误返回
    node->instances = sysconf_malloc(sizeof(sysconf_instance)*size);
    if (!node->instances)
    {
        log_trace_err(handle, "sysconf_parse_instances, oom");
        return SYSCONF_EC_OOM;
    }

    node->instanceNum = size;
    memset(node->instances, 0, sizeof(sysconf_instance)*size);

    // 遍历进程实例JSON数组，生成进程实例对象
    // 进程实例配置项：ID、TYPE、PARA
    for (i = 0; i < size; i++)
    {
        found = 0;
        jsoe = json_array_get_object(jsa, i);
        type = json_object_get_integer(jsoe, "TYPE");
        
        for (j = 0; j < sf->sm.processNum; j++)
        {
            if (sf->sm.processes[j].type == type)
            {
                node->instances[i].process = &sf->sm.processes[j];
                found = 1;
                break;
            }
        }

        if (found == 0)
        {
            return SYSCONF_EC_NOPROC;
        }

        node->instances[i].id = json_object_get_integer(jsoe, "ID");
        ec = sysconf_parse_string(jsoe, "PARA",
            node->instances[i].param, sizeof(node->instances[i].param));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }
    }

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_mqcli(JSON_Object *jso)
{
    int32_t size = 0, handle = -1, ec = SYSCONF_EC_OK;
    JSON_Array *jsa = NULL;
    JSON_Object *jsoo = NULL;
    sysconf_mq_client *mq = NULL;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;
    mq = &sf->mqcli;

    // 获取MQ客户端配置，失败错误返回
    jsoo = json_object_get_object(jso, "MQ-CLI");
    if (!jsoo)
    {
        log_trace_err(handle, "sysconf_parse_mqcli, no mq client conf");
        return SYSCONF_EC_NOEXIST;
    }

    // 解析MQ客户端配置，支持的配置项：
    // PORT、POOL_SIZE、TIMEOUT、HEART_BEAT_INTERNAL、SCHEMA、
    // USERNAME、PASSWORD、STRINGVALUE_PAIRS(KEY、VALUE)
    mq->port = json_object_get_integer(jsoo, "PORT");
    mq->poolSize = json_object_get_integer(jsoo, "POOL_SIZE");
    mq->timeout = json_object_get_integer(jsoo, "TIMEOUT");
    mq->heartbeatInterval = json_object_get_integer(jsoo, "HEART_BEAT_INTERNAL");

    ec = sysconf_parse_string(jsoo, "SCHEMA", mq->schema, sizeof(sf->mqcli.schema));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    ec = sysconf_parse_string(jsoo, "USERNAME", mq->userName, sizeof(mq->userName));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    ec = sysconf_parse_string(jsoo, "PASSWORD", mq->password, sizeof(mq->password));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    jsa = json_object_get_array(jsoo, "STRINGVALUE_PAIRS");
    if (jsa)
    {
        size = json_array_get_count(jsa);
        if (size > 0)
        {
            mq->pairs = sysconf_malloc(sizeof(sysconf_pair) * size);
            if (!mq->pairs)
            {
                log_trace_err(handle, "sysconf_parse_mqcli, oom");
                return SYSCONF_EC_OOM;
            }

            memset(mq->pairs, 0, sizeof(sysconf_pair) * size);
            mq->pairNum = size;

            ec = sysconf_parse_pairs(jsa, mq->pairs, mq->pairNum);
            if (ec != SYSCONF_EC_OK)
            {
                log_trace_err(handle, "sysconf_parse_mqcli, pair error");
                return ec;
            }
        }
    }

    // 给MQ客户端配置对象关联服务器网卡
    mq->nics = sysconf_make_native_nics(&mq->nicNum, 1, SYSCONF_PROC_TYPE_MQ);

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_rtdbcli(JSON_Object *jso)
{
    int32_t size = 0, handle = -1, ec = SYSCONF_EC_OK;
    JSON_Array *jsa = NULL;
    JSON_Object *jsoo = NULL;
    sysconf_rtdb_client *rtdb = NULL;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;
    rtdb = &sf->rtdbcli;

    // 获取RTDB API配置，失败错误返回
    jsoo = json_object_get_object(jso, "RTDB-CLI");
    if (!jsoo)
    {
        log_trace_err(handle, "sysconf_parse_rtdbcli, no rtdb api conf");
        return SYSCONF_EC_NOEXIST;
    }

    // 解析RTDB API配置，支持的配置项：
    // RTDB_PORT、SAFE_PORT、USERNAME、PASSWORD、STRINGVALUE_PAIRS(KEY、VALUE)
    rtdb->rtdbPort = json_object_get_integer(jsoo, "RTDB_PORT");
    rtdb->safePort = json_object_get_integer(jsoo, "SAFE_PORT");

    ec = sysconf_parse_string(jsoo, "USERNAME", rtdb->userName, sizeof(rtdb->userName));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    ec = sysconf_parse_string(jsoo, "PASSWORD", rtdb->password, sizeof(rtdb->password));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    jsa = json_object_get_array(jsoo, "STRINGVALUE_PAIRS");
    if (jsa)
    {
        size = json_array_get_count(jsa);
        if (size > 0)
        {
            rtdb->pairs = sysconf_malloc(sizeof(sysconf_pair)*size);
            if (!rtdb->pairs)
            {
                log_trace_err(handle, "sysconf_parse_rtdbcli, oom");
                return SYSCONF_EC_OOM;
            }

            memset(rtdb->pairs, 0, sizeof(sysconf_pair)*size);
            rtdb->pairNum = size;

            ec = sysconf_parse_pairs(jsa, rtdb->pairs, rtdb->pairNum);
            if (ec != SYSCONF_EC_OK)
            {
                log_trace_err(handle, "sysconf_parse_rtdbcli, pair error");
                return ec;
            }
        }
    }

    // 给RTDB API配置对象关联服务器网卡
    rtdb->nics = sysconf_make_native_nics(&rtdb->nicNum, 2,
        SYSCONF_PROC_TYPE_RTDB, SYSCONF_PROC_TYPE_SAFE);

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_tsdbcli(JSON_Object *jso)
{
    int32_t size = 0, handle = -1, ec = SYSCONF_EC_OK;
    JSON_Object *jsoo = NULL;
    sysconf_tsdb_client *tsdb = NULL;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;
    tsdb = &sf->tsdbcli;

    // 获取TSDB客户端配置，失败错误返回
    jsoo = json_object_get_object(jso, "TSDB-CLI");
    if (!jsoo)
    {
        log_trace_err(handle, "sysconf_parse_tsdbcli, no tsdb client conf");
        return SYSCONF_EC_NOEXIST;
    }

    // 解析TSDB客户端配置，支持的配置项：
    // PORT、TIMEOUT、HEART_BEAT_INTERNAL、SCHEMA、USERNAME、PASSWORD、DATABASE
    tsdb->port = json_object_get_integer(jsoo, "PORT");
    tsdb->timeout = json_object_get_integer(jsoo, "TIMEOUT");
    tsdb->heartbeatInterval = json_object_get_integer(jsoo, "HEART_BEAT_INTERNAL");

    ec = sysconf_parse_string(jsoo, "SCHEMA", tsdb->schema, sizeof(tsdb->schema));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    ec = sysconf_parse_string(jsoo, "USERNAME", tsdb->userName, sizeof(tsdb->userName));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    ec = sysconf_parse_string(jsoo, "PASSWORD", tsdb->password, sizeof(tsdb->password));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    ec = sysconf_parse_string(jsoo, "DATABASE", tsdb->database, sizeof(tsdb->database));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    // 给TSDB客户端配置对象关联服务器网卡
    tsdb->nics = sysconf_make_native_nics(&tsdb->nicNum, 1, SYSCONF_PROC_TYPE_TSDB);

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_cfdbcli(JSON_Object *jso)
{
    int32_t size = 0, handle = -1, ec = SYSCONF_EC_OK;
    JSON_Object *jsoo = NULL;
    sysconf_cfdb_client *cfdb = NULL;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;
    cfdb = &sf->cfdbcli;

    // 获取配置服务配置，失败错误返回
    jsoo = json_object_get_object(jso, "CFDB-CLI");
    if (!jsoo)
    {
        log_trace_err(handle, "sysconf_parse_cfdbcli, no cfdb client conf");
        return SYSCONF_EC_NOEXIST;
    }

    // 解析配置服务配置，支持的配置项：
    // SYNC_PORT、CONF_PORT、URL_PATH
    cfdb->syncPort = json_object_get_integer(jsoo, "SYNC_PORT");
    cfdb->confPort = json_object_get_integer(jsoo, "CONF_PORT");

    ec = sysconf_parse_string(jsoo, "URL_PATH", cfdb->urlPath, sizeof(cfdb->urlPath));
    if (ec == SYSCONF_EC_TOLONG)
    {
        return ec;
    }

    // 给配置服务配置对象关联服务器网卡
    cfdb->nics = sysconf_make_native_nics(&cfdb->nicNum, 1, SYSCONF_PROC_TYPE_CFDB);

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_pmcli(JSON_Object *jso)
{
    int32_t size = 0, handle = -1;
    JSON_Object *jsoo = NULL;
    sysconf_pm_client *pm = NULL;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;
    pm = &sf->pmcli;

    // 获取进程管理配置，失败错误返回
    jsoo = json_object_get_object(jso, "PM-CLI");
    if (!jsoo)
    {
        log_trace_err(handle, "sysconf_parse_pmcli, no process manage");
        return SYSCONF_EC_NOEXIST;
    }

    // 解析进程管理配置，支持的配置项：
    // PORT、SWITCH_PERIOD、LOGON_PERIOD、MAX_CLIENTS、MAX_IDLETIME、
    // TCP_BACKLOG、TCP_KEEPALIVE、BACKGROUND、LOCAL_DOMAIN
    pm->port = json_object_get_integer(jsoo, "PORT");
    pm->switchPeriod = json_object_get_integer(jsoo, "SWITCH_PERIOD");
    pm->logonPeriod = json_object_get_integer(jsoo, "LOGON_PERIOD");
    pm->maxClients = json_object_get_integer(jsoo, "MAX_CLIENTS");
    pm->maxIdleTime = json_object_get_integer(jsoo, "MAX_IDLETIME");
    pm->tcpBacklog = json_object_get_integer(jsoo, "TCP_BACKLOG");
    pm->tcpKeepAlive = json_object_get_integer(jsoo, "TCP_KEEPALIVE");
    pm->backGround = json_object_get_integer(jsoo, "BACKGROUND");
    pm->localDomain = json_object_get_integer(jsoo, "LOCAL_DOMAIN");

    // 给进程管理配置对象关联服务器网卡
    pm->nics = sysconf_make_native_nics(&pm->nicNum, 1, SYSCONF_PROC_TYPE_PM);

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_pairs(JSON_Array *jsa, sysconf_pair *pairs, int32_t size)
{
    int32_t i = 0, ec = SYSCONF_EC_OK;
    JSON_Object *jso = 0;

    for (i = 0; i < size; i++)
    {

        jso = json_array_get_object(jsa, i);
        ec = sysconf_parse_string(jso, "KEY", pairs[i].key, sizeof(pairs[i].key));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }
        ec = sysconf_parse_string(jso, "VALUE", pairs[i].value, sizeof(pairs[i].value));
        if (ec == SYSCONF_EC_TOLONG)
        {
            return ec;
        }
    }

    return SYSCONF_EC_OK;
}

static int32_t sysconf_parse_string(JSON_Object *jso, const char *key, char *val, int32_t vallen)
{
    int32_t size = 0, handle = -1;
    const char *cstr = NULL;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return SYSCONF_EC_NOINIT;
    }

    handle = sf->log;

    // 检查JSON对象是否包含"key"
    if (!json_object_has_value(jso, key))
    {
        log_trace_err(handle, "sysconf_parse_string, key(%s) not exist", key);
        return SYSCONF_EC_NOEXIST;
    }

    cstr = json_object_get_string(jso, key);
    if (cstr)
    {
        size = strlen(cstr);
        if (size > vallen)
        {
            log_trace_err(handle, "sysconf_parse_string, key(%s)'s value(%s) to long", key, cstr);
            return SYSCONF_EC_TOLONG;
        }
        memcpy(val, cstr, size);
    }
    else
    {
        log_trace_debug(handle, "sysconf_parse_string, key(%s)'s value empty", key);
    }

    return SYSCONF_EC_OK;
}

static int32_t sysconf_is_instance_exist(const sysconf_node *node, int32_t typenum, va_list types)
{
    int32_t i = 0, j = 0, type = 0, exist = 0;

    // 检查节中是否存在指定类型的进程实例，
    // 只要“types”中指定的任何一个进程实例在节点中存在，就认为存在。
    for (i = 0; i < node->instanceNum; i++)
    {
        for (j = 0; j < typenum; j++)
        {
            type = va_arg(types, int32_t);
            if ((node->instances[i].process) && (node->instances[i].process->type == type))
            {
                exist = 1;
                break;
            }
        }
        
        if (exist)
        {
            break;
        }
    }

    return exist;
}

static sysconf_nic** sysconf_make_native_nics(int32_t *size, int32_t typenum, ...)
{
    int32_t i = 0, j = 0, count = 0, idx = 0, nodenum = 0;
    sysconf_node **nodes = NULL;
    sysconf_conf *sf = NULL;
    sysconf_nic **nics = NULL;
    va_list ap;
    
    // 检查输入参数、为输出参数设初始值
    if (!size)
    {
        return NULL;
    }

    *size = 0;

    if (typenum == 0)
    {
        return NULL;
    }

    // 检查配置是否成功初始化
    sf = sysconf_get_sysconf();
    if (!sf)
    {
        return NULL;
    }

    if ((!sf->sm.native) || (!sf->sm.native->domain))
    {
        log_trace_warn(sf->log, "sysconf_make_native_nics, native empty");
        return NULL;
    }

    // 统计进程实例所在节点的网卡数量(仅针对本地域下的节点)
    count = 0;
    nodenum = sf->sm.native->domain->nodeNum;
    nodes = sf->sm.native->domain->nodes;
    for (i = 0; i < nodenum; i++)
    {
        va_start(ap, typenum);
        if (sysconf_is_instance_exist(nodes[i], typenum, ap))
        {
            count += nodes[i]->nicNum;
        }
        va_end(ap);
    }

    if (count == 0)
    {
        log_trace_warn(sf->log, "sysconf_make_native_nics, no nic");
        return NULL;
    }

    // 创建网卡数组，设置网卡对象
    nics = sysconf_malloc(sizeof(sysconf_nic*)*count);
    if (!nics)
    {
        log_trace_err(sf->log, "sysconf_make_native_nics, oom");
        return NULL;
    }

    idx = 0;
    *size = count;
    memset(nics, 0, sizeof(sysconf_nic*)*count);

    for (i = 0; i < nodenum; i++)
    {
        va_start(ap, typenum);
        if (sysconf_is_instance_exist(nodes[i], typenum, ap))
        {
            for (j = 0; j < nodes[i]->nicNum; j++)
            {
                nics[idx++] = &nodes[i]->nics[j];
            }
        }
        va_end(ap);
    }

    return nics;
}

static int32_t sysconf_get_hostname(char *hostname, int32_t len)
#ifdef _WIN32
{
    int32_t status = 0, handle = -1;
    sysconf_conf *sf = NULL;
    WSADATA wsaData;
    
    sf = sysconf_get_sysconf();
    if (sf)
    {
        handle = sf->log;
    }

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    status = gethostname(hostname, len);
    WSACleanup();
    if (status)
    {
        log_trace_err(handle, "sysconf_get_hostname, return=%d, error=%d, desc=%s", status, errno, strerror(errno));
        status = SYSCONF_EC_ERR;
    }
    else
    {
        log_trace_debug(handle, "sysconf_get_hostname, status=%d, hostname=%s", status, hostname);
        status = SYSCONF_EC_OK;
    }

    return status;
}
#else
{
    int32_t status = 0, handle = -1;
    sysconf_conf *sf = NULL;
    
    sf = sysconf_get_sysconf();
    if (sf)
    {
        handle = sf->log;
    }

    status = gethostname(hostname, len);
    if (status)
    {
        log_trace_err(handle, "sysconf_get_hostname, return=%d, error=%d, desc=%s", status, errno, strerror(errno));
        status = SYSCONF_EC_ERR;
    }
    else
    {
        log_trace_debug(handle, "sysconf_get_hostname, status=%d, hostname=%s", status, hostname);
        status = SYSCONF_EC_OK;
    }

    return status;
}
#endif
