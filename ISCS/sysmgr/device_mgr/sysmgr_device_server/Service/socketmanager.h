#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>

class SocketManager : public QObject
{
        Q_OBJECT
    public:
        explicit SocketManager(QObject *parent = 0);

        void setSocket(QTcpSocket* sock);
    signals:
        void recvCommandSignal(QTcpSocket* socket,QByteArray command);
    public slots:

        void disconnectedSlot();
        void errorSlot(QAbstractSocket::SocketError socketError);
        void readyReadSlot();

        void sendResponseSlot(QTcpSocket* client,QByteArray response);
    private:
        QTcpSocket* socket;
};

#endif // SOCKETMANAGER_H
