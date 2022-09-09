#ifndef COMMUNICATESERVERWORKERIMPL_H
#define COMMUNICATESERVERWORKERIMPL_H

#include <QObject>
#include "communicateserverworker.h"
#include "commandHandle/commandhandlethread.h"

class CommunicateServerWorkerImpl : public CommunicateServerWorker
{
        Q_OBJECT
    public:
        explicit CommunicateServerWorkerImpl(QObject *parent = 0);

        void start();
        void setConfManager(ConfManager* manager);
        void setInfoManager(DevInfoManager* manager);
        void setCommandManager(CommandDataManager* manager);
        bool startTcpServer();
        void startCommunicate();
        void startSendMessage();
    protected:
        void incomingConnection(qintptr socketDescriptor);
    signals:

    public slots:

    private:
        ConfManager* confManager;
        CommandHandleThread* commandThread;
        DevInfoManager* infoManager;
        CommandDataManager* commandManager;

        void init();
        void initParam();
};

#endif // COMMUNICATESERVERWORKERIMPL_H
