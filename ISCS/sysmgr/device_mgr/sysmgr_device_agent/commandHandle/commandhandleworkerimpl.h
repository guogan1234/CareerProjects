#ifndef COMMANDHANDLEWORKERIMPL_H
#define COMMANDHANDLEWORKERIMPL_H

#include <QObject>
#include "commandhandleworker.h"

#include "data/sendmsg.h"

class CommandHandleWorkerImpl : public CommandHandleWorker
{
        Q_OBJECT
    public:
        explicit CommandHandleWorkerImpl(QObject *parent = 0);

        void setCommandManager(CommandDataManager* manager);
        void setDevInfoManager(DevInfoManager* manager);
        int handleCommand();     
    signals:

    public slots:
    private:
        CommandDataManager* commandManager;
        DevInfoManager* infoManager;

        void takeSetCommandManager(CommandDataManager *manager);
        void takeSetDevInfoManager(DevInfoManager *manager);

        int handleDataCommand(QTcpSocket* socket,SendMsg* sendCommand);
        int handleConfCommand(SendMsg* sendCommand);
};

#endif // COMMANDHANDLEWORKERIMPL_H
