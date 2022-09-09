#ifndef SOCKETWRITERWORKERIMPL_H
#define SOCKETWRITERWORKERIMPL_H

#include <QObject>
#include <QTcpSocket>
#include "MsgHandle/sendmsgworkerimpl.h"

class SocketWriterWorkerImpl : public QObject
{
    Q_OBJECT
public:
    explicit SocketWriterWorkerImpl(QObject *parent = nullptr);

    void setSocket(QTcpSocket* socket);
    int sendDataRequest();
signals:

public slots:
    void dataTimoutSlot(qint64 timestamp);

private:
    QTcpSocket* socket;
    SendMsgWorkerImpl* sendMsgImpl;

    void init();
    void initParam();

    int takeSendDataRequest(qint64 timestamp);
};

#endif // SOCKETWRITERWORKERIMPL_H
