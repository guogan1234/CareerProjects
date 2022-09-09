#include "infoserverworkerimpl.h"
#include "ConfHandle/confhandleworkimpl.h"
#include "Enum/enumtypes.h"
#include "Servers/stationserverimpl.h"
#include "Servers/centerserverimpl.h"

InfoServerWorkerImpl::InfoServerWorkerImpl()
{
    init();
}

int InfoServerWorkerImpl::start()
{
    int ret = -1;
    ret = startConfHandle();
    if(-1 == ret) return -1;
    ret = startServer();
    if(-1 == ret) return -1;
    ret = startCommunicate();
    if(-1 == ret) return -1;

    return ret;
}

int InfoServerWorkerImpl::startConfHandle()
{
    confWorker->getWorkPCList();
    confWorker->getLocalDomainList();

    return 0;
}

int InfoServerWorkerImpl::startServer()
{
    switch (domain) {
    case Station:
    {
        //获取所有工作站(节点类型为3)连接信息，并启动
        StationServerImpl* server = new StationServerImpl(confWorker->getLocalDomainInfo());
        server->setParent(this);
        server->setConfHandleWorker(confWorker);
        server->start();
        break;
    }
    case Center:
    {
        CenterServerImpl* server = new CenterServerImpl(confWorker->getWorkPCsInfo());
        server->setParent(this);
        server->start();
        break;
    }
    default:
        break;
    }
    return 0;
}

int InfoServerWorkerImpl::startCommunicate()
{
    return 0;
}

int InfoServerWorkerImpl::startInfoHandle()
{
    return 0;
}

int InfoServerWorkerImpl::startService()
{
    return 0;
}

int InfoServerWorkerImpl::init()
{
    initParam();

    return 0;
}

int InfoServerWorkerImpl::initParam()
{
    domain = 0;//0 - station,1 - center
    confWorker = new ConfHandleWorkImpl(this);

    return 0;
}
