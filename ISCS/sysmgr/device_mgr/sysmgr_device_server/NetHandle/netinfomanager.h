#ifndef NETINFOMANAGER_H
#define NETINFOMANAGER_H

#include <QObject>
#include <QMap>
#include <QReadWriteLock>
#include "Data/netinfo.h"

class NetInfoManager : public QObject
{
    Q_OBJECT
public:
    explicit NetInfoManager(QObject *parent = nullptr);

    void setNetInfoMap(QMap<QString,NetInfo*> * map,QReadWriteLock* locker);
    int addNetInfo(qint64 timestamp,NetInfo* data);
signals:

public slots:

private:
    QMap<QString,NetInfo*> * infoMap;
    QReadWriteLock* mapLocker;
    void init();
    void initParam();

    void takeSetNetInfoMap(QMap<QString,NetInfo*> * map,QReadWriteLock* locker);
    int takeAddNetInfo(qint64 timestamp,NetInfo* data);
};

#endif // NETINFOMANAGER_H
