#ifndef COMMUNICATESERVERWORKER_H
#define COMMUNICATESERVERWORKER_H

#include <QObject>
#include <QTcpServer>
#include "deviceInfo/devinfomanager.h"
#include "commandManager/commanddatamanager.h"
#include "conf/confmanager.h"

class CommunicateServerWorker : public QTcpServer
{
    Q_OBJECT
public:
    explicit CommunicateServerWorker(QObject *parent = 0);

    virtual void start() = 0;
    virtual void setConfManager(ConfManager* manager) = 0;
    virtual void setInfoManager(DevInfoManager* manager) = 0;
    virtual void setCommandManager(CommandDataManager* manager) = 0;
    virtual bool startTcpServer() = 0;
    virtual void startCommunicate() = 0;
    virtual void startSendMessage() = 0;
signals:

public slots:
};

#endif // COMMUNICATESERVERWORKER_H
