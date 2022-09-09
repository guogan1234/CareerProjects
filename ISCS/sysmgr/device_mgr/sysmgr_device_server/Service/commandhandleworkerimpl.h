#ifndef COMMANDHANDLEWORKERIMPL_H
#define COMMANDHANDLEWORKERIMPL_H

#include <QObject>
#include <QTcpSocket>
#include "datafinder.h"

class CommandHandleWorkerImpl : public QObject
{
        Q_OBJECT
    public:
        explicit CommandHandleWorkerImpl(QObject *parent = 0);

        void setDataFinder(DataFinder* finder);
    signals:
        void sendResponseSignal(QTcpSocket* client,QByteArray response);
    public slots:
        void recvMessageSlot(QTcpSocket* client,QByteArray msg);
    private:
        DataFinder* dataFinder;
};

#endif // COMMANDHANDLEWORKERIMPL_H
