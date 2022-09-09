#ifndef COLLECTSERVERWORKER_H
#define COLLECTSERVERWORKER_H

#include <QObject>
#include "deviceInfo/devinfomanager.h"
#include "conf/confmanager.h"

class CollectServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit CollectServerWorker(QObject *parent = 0);

    virtual void start() = 0;
    virtual void setInfoManager(DevInfoManager* manager) = 0;
    virtual void setConfManager(ConfManager* manager) = 0;
signals:

public slots:
private:
    virtual void startThread() = 0;
    virtual void collectInfo() = 0;
};

#endif // COLLECTSERVERWORKER_H
