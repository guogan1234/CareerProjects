#ifndef CONFIGSERVERIMPL_H
#define CONFIGSERVERIMPL_H

#include <QTcpServer>
#include <QTcpSocket>
#include "ConfHandle/configworkerimpl.h"

class ConfigServerImpl : public QTcpServer
{
    Q_OBJECT
public:
    explicit ConfigServerImpl(QObject *parent = 0);

    void start();
    void incomingConnection(qintptr handle);
signals:
    void dataSignal(QByteArray datas);
public slots:
    void readSlot();
private:
    QTcpSocket* socket;
    ConfigWorkerImpl* configWorker;

    void init();
};

#endif // CONFIGSERVERIMPL_H
