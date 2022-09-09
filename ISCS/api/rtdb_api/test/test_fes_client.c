#include "rtdbcli_client.h"
#include "rtdbcli_command.h"
#include "rtdbcli_fes.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define FES_ID  "rtu_01 rtu_02"

typedef struct
{
    int8_t  used;
    int32_t domain;
    int32_t type;
    char *id;
    char *dest;
    char *data;
} command_data;

static command_data _cmddata[1024] = {0};
static int32_t _cmdmaxsize = sizeof(_cmddata)/sizeof(command_data);
static pthread_t _cmdthread = 0;
static pthread_t _datathread = 0;
static pthread_mutex_t _cmdmtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _datamtx = PTHREAD_MUTEX_INITIALIZER;

static int64_t get_microsecond();
static int32_t upload_single_soe(int32_t h);
static int32_t upload_multi_soes(int32_t h);
static int32_t upload_single_datapoint(int32_t h, int32_t type);
static int32_t upload_multi_datapoints(int32_t h, int32_t type);
static int32_t register_command(int32_t h);
static int32_t append_command(int32_t domain, int32_t type, const char *dest, const char *id, const char *data);
static command_data* fetch_command();
static void free_command(command_data *cd);
static void command_receiver(int32_t domain, int32_t type, const char *dest, const char *id, const char *cmd, const void *ud);
static void* cmd_workrun(void *arg);
static void* data_workrun(void *arg);

// 支持的输入参数有：
// --safe    安全FES客户端
// --unsafe  非安全FES客户端
int main(int argc, char *argv[])
{
    int32_t i = 0;
    int32_t h = -1;
    int32_t ec = RTDBCLI_EC_OK;
    int32_t type = RTDBCLI_TYPE_FES;

    // 输入参数处理
    for (i = 1; i < argc; i++)
    {
        if (strcasecmp(argv[1], "--safe") == 0)
        {
            type = RTDBCLI_TYPE_SFES;
            break;
        }
    }

    // 创建FES客户端
    ec = rtdbcli_create_client(type, &h);
    if (ec != RTDBCLI_EC_OK)
    {
        printf("create fes client(type=%d) error(%d)\n", type, ec);
        return ec;
    }

    // 创建命令接收线程
    ec = pthread_create(&_cmdthread, NULL, cmd_workrun, (void*)h);
    if (ec != 0)
    {
        printf("create command thread error(%s), %s\n", errno, strerror(errno));
        rtdbcli_free_client(h);
        return ec;
    }
    pthread_detach(_cmdthread);

    // 创建采样上报线程
    ec = pthread_create(&_datathread, NULL, data_workrun, (void*)h);
    if (ec != 0)
    {
        printf("create data thread error(%s), %s\n", errno, strerror(errno));
        rtdbcli_free_client(h);
        return ec;
    }
    pthread_detach(_datathread);

    // 等待输入控制信号
    while (getchar() != 'q')
    {
        ;
    }

    rtdbcli_free_client(h);

    return 0;
}

static int64_t get_microsecond()
{
    int64_t ms = 0;
    struct timeval tv = {0, 0};

    gettimeofday(&tv, NULL);
    ms = tv.tv_sec*1000*1000 + tv.tv_usec;
    
    return ms;
}

static int32_t upload_single_soe(int32_t h)
{
    int32_t ec = RTDBCLI_EC_OK;
    rtdbcli_soe *psoe = NULL;

    // 检查FES客户端有效性
    if (h == -1)
    {
        printf("fes client handle invalid\n");
        return RTDBCLI_EC_ERROR;
    }

    // 为SOE准备缓存
    ec = rtdbcli_prepare_soe(h, &psoe);
    if ((ec != RTDBCLI_EC_OK) || (psoe == NULL))
    {
        printf("prepare buffer for soe error(%d)\n", ec);
        return ec;
    }

    // 设置SOE数据
    psoe->timestamp = get_microsecond();
    memcpy(psoe->path, "a:b:c:e:f", 9);
    memcpy(psoe->timemark, "12345.567", 9);
    memcpy(psoe->value, "2", 1);

    // 上传SOE
    ec = rtdbcli_upload_soe(h, psoe);
    if (ec == RTDBCLI_EC_OK)
    {
        printf("upload soe ok\n");
    }
    else
    {
        printf("upload soe error(%d)\n", ec);
    }

    // 释放SOE缓存
    rtdbcli_destory_soe(h, psoe);

    return ec;
}

static int32_t upload_multi_soes(int32_t h)
{
    int32_t ec = RTDBCLI_EC_OK;
    int32_t i, soesize = 0;
    rtdbcli_soe *psoes = NULL;

    // 检查FES客户端有效性
    if (h == -1)
    {
        printf("fes client handle invalid\n");
        return RTDBCLI_EC_ERROR;
    }

    // 为SOE准备缓存
    ec = rtdbcli_prepare_soes(h, &psoes, &soesize);
    if ((ec != RTDBCLI_EC_OK) || (psoes == NULL) || (soesize == 0))
    {
        printf("prepare buffer for soe array error(%d)\n", ec);
        return ec;
    }

    // 设置SOE数据
    for (i = 0; i < soesize; i++)
    {
        psoes[i].timestamp = get_microsecond();
        snprintf(psoes[i].path, sizeof(psoes[i].path), "a:b:c:d:e%d", i+1);
        snprintf(psoes[i].timemark, sizeof(psoes[i].timemark), "123%d.456", i+1);
        snprintf(psoes[i].value, sizeof(psoes[i].value), "%d", i+1);
    }

    // 上传SOE
    ec = rtdbcli_upload_soes(h, psoes, soesize);
    if (ec == RTDBCLI_EC_OK)
    {
        printf("upload soe ok\n");
    }
    else
    {
        printf("upload soe error(%d)\n", ec);
    }

    // 释放SOE缓存
    rtdbcli_destory_soes(h, psoes, soesize);

    return ec;
}

static int32_t upload_single_datapoint(int32_t h, int32_t type)
{
    int32_t ec = RTDBCLI_EC_OK;
    rtdbcli_datapoint *pdp = NULL;

    // 检查FES客户端有效性
    if (h == -1)
    {
        printf("fes client handle invalid\n");
        return RTDBCLI_EC_ERROR;
    }

    if ((type != RTDBCLI_FES_AI) && (type == RTDBCLI_FES_DI) && (type == RTDBCLI_FES_ACC))
    {
        printf("invalid type(%d)\n", type);
        return RTDBCLI_EC_NOTSUPPORTED;
    }

    // 准备数据点缓存
    ec = rtdbcli_prepare_datapoint(h, &pdp);
    if ((ec != RTDBCLI_EC_OK) || (pdp == NULL))
    {
        printf("prepare buffer for datapoint error(%d)\n", ec);
        return ec;
    }

    // 设置数据
    pdp->timestamp = get_microsecond();
    pdp->type = type;
    pdp->isChanged = 0;
    pdp->status = 0;
    if (type == RTDBCLI_FES_AI)
    {
        pdp->real = (rand()%100)*0.123;
    }
    else if (type == RTDBCLI_FES_DI)
    {
        pdp->integer = (((rand()%2) == 1) ? 1 : 0);
    }
    else
    {
        static int32_t sval = 1;
        pdp->integer = ++sval;
    }

    // 上传点采样数据
    ec = rtdbcli_upload_datapoint(h, pdp);
    if (ec == RTDBCLI_EC_OK)
    {
        printf("upload datapoint(type=%d) ok\n", type);
    }
    else
    {
        printf("upload datapoint(type=%d) error(%d)\n", type, ec);
    }

    // 释放采样数据点缓存
    rtdbcli_destory_datapoint(h, pdp);

    return ec;
}

static int32_t upload_multi_datapoints(int32_t h, int32_t type)
{
    int32_t ec = RTDBCLI_EC_OK;
    int32_t i = 0, dpsize = 0;
    rtdbcli_datapoint *pdps = NULL;

    // 检查FES客户端有效性
    if (h == -1)
    {
        printf("fes client handle invalid\n");
        return RTDBCLI_EC_ERROR;
    }

    if ((type != RTDBCLI_FES_AI) && (type == RTDBCLI_FES_DI) && (type == RTDBCLI_FES_ACC))
    {
        printf("invalid type(%d)\n", type);
        return RTDBCLI_EC_NOTSUPPORTED;
    }

    // 准备数据点缓存
    ec = rtdbcli_prepare_datapoints(h, &pdps, &dpsize);
    if ((ec != RTDBCLI_EC_OK) || (pdps == NULL) || (dpsize == 0))
    {
        printf("prepare buffer for datapoint array error(%d)\n", ec);
        return ec;
    }

    // 设置数据
    for (i = 0; i < dpsize; i++)
    {
        pdps[i].timestamp = get_microsecond();
        pdps[i].type = type;
        pdps[i].isChanged = 0;
        pdps[i].status = 0;
        if (type == RTDBCLI_FES_AI)
        {
            pdps[i].real = (rand() % 100) * 0.123;
        }
        else if (type == RTDBCLI_FES_DI)
        {
            pdps[i].integer = (((rand() % 2) == 1) ? 1 : 0);
        }
        else
        {
            static int32_t sval = 1;
            pdps[i].integer = ++sval;
        }
    }

    // 上传点采样数据
    ec = rtdbcli_upload_datapoints(h, pdps, dpsize);
    if (ec == RTDBCLI_EC_OK)
    {
        printf("upload datapoints (type=%d) ok\n", type);
    }
    else
    {
        printf("upload datapoints(type=%d) error(%d)\n", type, ec);
    }

    // 释放采样数据点缓存
    rtdbcli_destory_datapoints(h, pdps, dpsize);

    return ec;
}

static int32_t register_command(int32_t h)
{
    int32_t ec = 0;

    // 注册命令接收函数，用于通知命令
    ec = rtdbcli_register_command_notifier(h, command_receiver, NULL);
    if (ec != RTDBCLI_EC_OK)
    {
        printf("register command notifier error(%d)\n", ec);
        return ec;
    }

    // 1) 注册命令服务，用于接收服务器下发的命令
    ec = rtdbcli_register_service(h, FES_ID);
    if (ec != RTDBCLI_EC_OK)
    {
        printf("register command service error(%d)\n", ec);
        return ec;
    }

    return RTDBCLI_EC_OK;
}

static int32_t append_command(int32_t domain, int32_t type, const char *dest, const char *id, const char *data)
{
    int32_t ec = RTDBCLI_EC_OK;
    command_data *pcmd = NULL;

    pthread_mutex_lock(&_cmdmtx);
    for (int32_t i = 0, len = 0; i < _cmdmaxsize; i++)
    {
        if (_cmddata[i].used == 0)
        {
            pcmd = &_cmddata[i];
            break;
        }
    }

    if (pcmd)
    {
        pcmd->type = type;
        pcmd->domain = domain;
        if (dest)
        {
            pcmd->dest = strdup(dest);
        }
        if (id)
        {
            pcmd->id = strdup(id);
        }
        if (data)
        {
            pcmd->data = strdup(data);
        }
    }
    else
    {
        ec = RTDBCLI_EC_NOTEXIST;
    }
    pthread_mutex_unlock(&_cmdmtx);

    return ec;
}

static command_data* fetch_command()
{
    command_data *data = NULL;

    pthread_mutex_lock(&_cmdmtx);
    for (int32_t i = 0; i < _cmdmaxsize; i++)
    {
        if (_cmddata[i].used != 0)
        {
            data = &_cmddata[i];
            break;
        }
    }
    pthread_mutex_unlock(&_cmdmtx);

    return data;
}

static void free_command(command_data *cd)
{
    pthread_mutex_lock(&_cmdmtx);
    if (cd)
    {
        if (cd->dest)
        {
            free(cd->dest);
        }
        if (cd->id)
        {
            free(cd->id);
        }
        if (cd->data)
        {
            free(cd->data);
        }
        memset(cd, 0, sizeof(command_data));
    }
    pthread_mutex_unlock(&_cmdmtx);
}

static void command_receiver(int32_t domain, int32_t type, const char *dest, const char *id, const char *cmd, const void *ud)
{
    int32_t ec = RTDBCLI_EC_OK;

    printf("receive command: domain=%d, type=%d, destination=%s, id=%s, cmd=%s\n", domain, type, dest, id, cmd);
    ec = append_command(domain, type, dest, id, cmd);
    if (ec != RTDBCLI_EC_OK)
    {
        printf("add command to buffer error\n");
    }
}

static void* cmd_workrun(void *arg)
{
    int32_t h = (int32_t)arg;
    int32_t ec = RTDBCLI_EC_OK;
    command_data *pcd = NULL;

    if (register_command(h) != RTDBCLI_EC_OK)
    {
        printf("regiest command error\n");
        return (void*)-1;
    }

    while (1)
    {
        pcd = fetch_command();
        if (pcd)
        {
            ec = rtdbcli_send_command_result(h, pcd->domain, pcd->type, pcd->dest, pcd->id, 0, "{\"msg\":\"ok\"}");
            if (ec == RTDBCLI_EC_OK)
            {
                printf("send command result ok\n");
            }
            else
            {
                printf("send command result error(%d)\n", ec);
            }

            free_command(pcd);
        }
        else
        {
            sleep(1);
        }
    }

    return (void*)0;
}

static void* data_workrun(void *arg)
{
    int32_t h = (int32_t)arg;
    int32_t ec = RTDBCLI_EC_OK;
    int32_t i = 0;
    int32_t types[] = {RTDBCLI_FES_AI, RTDBCLI_FES_DI, RTDBCLI_FES_ACC};

    while (1)
    {
        // 上传SOE
        ec = upload_single_soe(h);
        if (ec == RTDBCLI_EC_OK)
        {
            printf("data_workrun, upload soe ok\n");
        }
        else
        {
            printf("data_workrun, upload soe error(%d)\n", ec);
        }

        // 批量上传SOE
        ec = upload_single_soe(h);
        if (ec == RTDBCLI_EC_OK)
        {
            printf("data_workrun, upload soes ok\n");
        }
        else
        {
            printf("data_workrun, upload soes error(%d)\n", ec);
        }

        // 上传单点数据采样
        for (i = 0; i < sizeof(types)/sizeof(int32_t); i++)
        {
            ec = upload_single_datapoint(h, types[i]);
            if (ec == RTDBCLI_EC_OK)
            {
                printf("data_workrun, upload datapoint(type=%d) ok\n", types[i]);
            }
            else
            {
                printf("data_workrun, upload datapoint(type=%d) error(%d)\n", types[i], ec);
            }
        }

        // 批量上传数据采样
        for (i = 0; i < sizeof(types)/sizeof(int32_t); i++)
        {
            ec = upload_single_datapoints(h, types[i]);
            if (ec == RTDBCLI_EC_OK)
            {
                printf("data_workrun, upload datapoints(type=%d) ok\n", types[i]);
            }
            else
            {
                printf("data_workrun, upload datapoints(type=%d) error(%d)\n", types[i], ec);
            }
        }
    }

    return (void*)0;
}
