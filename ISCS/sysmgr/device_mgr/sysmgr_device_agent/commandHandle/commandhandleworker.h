#ifndef COMMANDHANDLEWORKER_H
#define COMMANDHANDLEWORKER_H

#include <QObject>
#include "commandManager/commanddatamanager.h"
#include "deviceInfo/devinfomanager.h"

class CommandHandleWorker : public QObject
{
        Q_OBJECT
    public:
        explicit CommandHandleWorker(QObject *parent = 0);

        virtual void setCommandManager(CommandDataManager* manager) = 0;
        virtual void setDevInfoManager(DevInfoManager* manager) = 0;
        virtual int handleCommand() = 0;
    signals:

    public slots:
};

#endif // COMMANDHANDLEWORKER_H
