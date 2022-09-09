#ifndef SERVICETHREAD_H
#define SERVICETHREAD_H

#include <QThread>
#include "socketmanager.h"

class ServiceThread : public QThread
{
        Q_OBJECT
    public:
        explicit ServiceThread(QObject *parent = 0);

        void setSockDescriptor(qintptr sockfd);
        void run();
    signals:
        void recvCommandSignal(QTcpSocket* socket,QByteArray command);
        void sendResponseSignal(QTcpSocket* client,QByteArray response);
    public slots:

    private:
        qintptr sock;
        SocketManager* socketManager;
};

#endif // SERVICETHREAD_H
