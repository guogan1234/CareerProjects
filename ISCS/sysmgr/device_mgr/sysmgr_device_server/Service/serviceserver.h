#ifndef SERVICESERVER_H
#define SERVICESERVER_H

#include <QTcpServer>
#include "commandhandlethread.h"
#include "ConfHandle/configmanager.h"

class ServiceServer : public QTcpServer
{
        Q_OBJECT
    public:
        explicit ServiceServer(QObject *parent = 0);

        void setRawList(QList<QByteArray> * list,QMutex* mutex);
        void setRawHash(QHash<QString,QByteArray> * hash);
        void setNetMap(QMap<QString,NetInfo*> * map,QReadWriteLock* lock);
        void setConfigManager(ConfigManager* manager);
        int start();

        void incomingConnection(qintptr handle);
    signals:

    public slots:

    private:
        CommandHandleThread* commandThread;
        ConfigManager* configManager;

        void init();
        void initParam();
};

#endif // SERVICESERVER_H
