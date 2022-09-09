#ifndef CONFSERVERWORKER_H
#define CONFSERVERWORKER_H

#include <QObject>
#include "ConfHandle/confhandleworkimpl.h"

class ConfServerWorker : public QObject
{
        Q_OBJECT
    public:
        explicit ConfServerWorker(QObject *parent = 0);

        virtual QHash<int,DomainNodes*> * getStartConf() = 0;
        virtual QList<ConnClient*> * getConnServer() = 0;
    signals:

    public slots:

    protected:
        ConfHandleWorker* confWorker;
};

#endif // CONFSERVERWORKER_H
