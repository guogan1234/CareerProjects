#ifndef COMMANDHANDLETHREAD_H
#define COMMANDHANDLETHREAD_H

#include <QObject>
#include <QThread>
#include "commandhandleworkerimpl.h"

class CommandHandleThread : public QThread
{
        Q_OBJECT
    public:
        explicit CommandHandleThread(QObject *parent = 0);

        void run();
        void setCommandManager(CommandDataManager* manager);
        void setInfoManager(DevInfoManager* manager);
    signals:

    public slots:

    private:
        CommandHandleWorkerImpl* worker;
        CommandDataManager* commandManager;
        DevInfoManager* infoManager;
};

#endif // COMMANDHANDLETHREAD_H
