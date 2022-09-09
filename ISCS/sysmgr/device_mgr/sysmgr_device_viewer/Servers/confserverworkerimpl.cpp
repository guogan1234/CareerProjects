#include "confserverworkerimpl.h"

ConfServerWorkerImpl::ConfServerWorkerImpl(QObject *parent) : ConfServerWorker(parent)
{
    init();
}

QHash<int, DomainNodes *> *ConfServerWorkerImpl::getStartConf()
{
    if(!confWorker) return NULL;
    return confWorker->getLocalDomainNodes();
}

QList<ConnClient *> *ConfServerWorkerImpl::getConnServer()
{
    if(!confWorker) return NULL;
    return confWorker->getLocalDomainServer();
}

void ConfServerWorkerImpl::init()
{
    initParam();
}

void ConfServerWorkerImpl::initParam()
{
    confWorker = new ConfHandleWorkImpl();
}
