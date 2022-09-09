#ifndef SOCKETMANAGERIMPL_H
#define SOCKETMANAGERIMPL_H

#include <QObject>
#include <QTcpSocket>
#include "Data/sysconfout.h"

class SocketManagerImpl : public QObject
{
        Q_OBJECT
    public:
        explicit SocketManagerImpl(QObject *parent = 0);

        void setConnServer(QList<ConnClient*> * server);
        bool connectServer();
        QTcpSocket* getConnSocket();
    signals:
        void recvDataSignal(QByteArray ba);
    public slots:       
        void connectedSlot();
        void disconnectedSlot();
        void errorSlot(QAbstractSocket::SocketError socketError);
        void readyReadSlot();
    private:
        //
        QList<ConnClient*> * connServer;
        QTcpSocket* socket;

        void initConnects();
};

#endif // SOCKETMANAGERIMPL_H
