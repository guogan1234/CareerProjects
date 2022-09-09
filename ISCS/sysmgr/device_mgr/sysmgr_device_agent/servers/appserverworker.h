#ifndef APPSERVERWORKER_H
#define APPSERVERWORKER_H

#include <QObject>

class AppServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit AppServerWorker(QObject *parent = 0);
    virtual void start() = 0;
signals:

public slots:

private:
    virtual void startLoadConfig() = 0;
    virtual void startCollectServer() = 0;
    virtual void startCommunicateServer() = 0;
};

#endif // APPSERVERWORKER_H
