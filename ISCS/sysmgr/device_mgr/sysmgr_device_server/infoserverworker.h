#ifndef INFOSERVERWORKER_H
#define INFOSERVERWORKER_H

#include <QObject>

class InfoServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit InfoServerWorker(QObject *parent = nullptr);

    virtual int start() = 0;

    virtual int startConfHandle() = 0;
    virtual int startServer() = 0;
    virtual int startCommunicate() = 0;
    virtual int startInfoHandle() = 0;
    virtual int startService() = 0;
signals:

public slots:
};

#endif // INFOSERVERWORKER_H
