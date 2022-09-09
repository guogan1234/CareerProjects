#ifndef COMMUNICATESOCKETWRITER_H
#define COMMUNICATESOCKETWRITER_H

#include <QObject>
#include <QTcpSocket>

class CommunicateSocketWriter : public QObject
{
        Q_OBJECT
    public:
        explicit CommunicateSocketWriter(QObject *parent = 0);

        void setSocket(QTcpSocket* socket);
        qint64 writeData(const QByteArray& data);
        QString getSocketError();
    signals:

    public slots:

    private:
        QTcpSocket* socket;
};

#endif // COMMUNICATESOCKETWRITER_H
