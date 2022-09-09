#ifndef NETTHREADIMPL_H
#define NETTHREADIMPL_H

#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include "Data/sysconfout.h"
#include "Data/netinfo.h"
#include "networkerimpl.h"
#include "netinfomanager.h"

class NetThreadImpl : public QThread
{
    Q_OBJECT
public:
    explicit NetThreadImpl();

    void run();
    void setNetInfoMap(QMap<QString,NetInfo*> * map,QReadWriteLock* locker);
    void setConnConfigs(QList<ConnClient*> * config);
signals:
    void dataTimeout(qint64);
public slots:

private:
    QList<ConnClient*> * connConfigs;
    QMap<QString,NetInfo*> * netInfoMap;
    QReadWriteLock* netMapLocker;
    NetWorkerImpl* netWorker;
    NetInfoManager* netInfoManager;

    void takeSetNetInfoMap(QMap<QString,NetInfo*> * map,QReadWriteLock* locker);
    void takeSetConnConfigs(QList<ConnClient*> * config);
};

#endif // NETTHREADIMPL_H
