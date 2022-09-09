#ifndef COMMUNICATESOCKET_H
#define COMMUNICATESOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "commandManager/commanddatamanager.h"

class CommunicateSocket : public QObject
{
        Q_OBJECT
    public:
        explicit CommunicateSocket(QObject *parent = 0);

        void setSocket(QTcpSocket* socket);
        void setCommandManager(CommandDataManager* manager);
        void initConnects();
    signals:

    public slots:
        void connectedSlot();
        void disconnectedSlot();
        void errorSlot(QAbstractSocket::SocketError socketError);
        void hostFoundSlot();
        void readyReadSlot();
    private:
        QTcpSocket* clientSocket;
        CommandDataManager* commandManager;

        void takeSetSocket(QTcpSocket *socket);
        void takeSetCommandManager(CommandDataManager *manager);
        void takeInitConnects();
};

#endif // COMMUNICATESOCKET_H
