#ifndef INFOSERVERWORKERIMPL_H
#define INFOSERVERWORKERIMPL_H

#include <QObject>
#include "infoserverworker.h"
#include "ConfHandle/confhandleworker.h"

class InfoServerWorkerImpl : public InfoServerWorker
{
public:
    InfoServerWorkerImpl();

    int start();

    int startConfHandle();
    int startServer();
    int startCommunicate();
    int startInfoHandle();
    int startService();

private:
    ConfHandleWorker* confWorker;

    int init();
    int initParam();

    int domain;
};

#endif // INFOSERVERWORKERIMPL_H
