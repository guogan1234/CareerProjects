#ifndef COMMANDHANDLETHREAD_H
#define COMMANDHANDLETHREAD_H

#include <QThread>
#include "commandhandleworkerimpl.h"
#include "commandmanager.h"

class CommandHandleThread : public QThread
{
        Q_OBJECT
    public:
        explicit CommandHandleThread(QObject *parent = 0);

        void setRawInfoList(QList<QByteArray> * list,QMutex* mutex);
        void setRawInfoHash(QHash<QString,QByteArray> * hash);
        void setNetInfoMap(QMap<QString,NetInfo*> * map,QReadWriteLock* lock);
        void run();
    signals:
        void recvCommandSignal(QTcpSocket* socket,QByteArray command);
        void sendResponseSignal(QTcpSocket* client,QByteArray response);
    public slots:

    private:
        DataFinder* finder;
        CommandManager* commandManager;
        CommandHandleWorkerImpl* worker;
        bool bRun;

        void init();
        void initParam();
};

#endif // COMMANDHANDLETHREAD_H
