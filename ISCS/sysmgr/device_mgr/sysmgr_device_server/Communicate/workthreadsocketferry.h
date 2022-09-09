#ifndef WORKTHREADSOCKETFERRY_H
#define WORKTHREADSOCKETFERRY_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>

class WorkThreadSocketFerry : public QObject
{
    Q_OBJECT
public:
    explicit WorkThreadSocketFerry(QObject *parent = nullptr);

    void setSocket(QTcpSocket* socket);
    void setConnIp(QString ip);
signals:
    void dataReady(QString,QByteArray);
public slots:
    int connectedSlot();
    int disconnectedSlot();
    int errorSlot(QAbstractSocket::SocketError socketError);
    int hostFoundSlot();
    int stateChangedSlot(QAbstractSocket::SocketState socketState);
    int aboutToCloseSlot();
    int bytesWrittenSlot(qint64 length);
    int readyReadSlot();
    int destroyedSlot(QObject* obj);

    void dataTimeoutSlot(qint64 timestamp);

private:
    QString getCurrentThreadId();

    QTcpSocket* socket;
    QString connIp;

    void takeSetSocket(QTcpSocket* socket);
};

#endif // WORKTHREADSOCKETFERRY_H
