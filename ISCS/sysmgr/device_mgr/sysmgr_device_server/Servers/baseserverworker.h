#ifndef BASESERVERWORKER_H
#define BASESERVERWORKER_H

#include <QObject>
#include "Data/sysconfout.h"

class BaseServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit BaseServerWorker(QObject *parent = nullptr);

    virtual int start() = 0;
signals:

    protected:
//        QHash<QString,ClientDataHandle*> clientHash;

public slots:

private:
    virtual int startThreads() = 0;
    virtual int startCommunicate() = 0;
};

#endif // BASESERVERWORKER_H
