#ifndef CONFSERVERWORKERIMPL_H
#define CONFSERVERWORKERIMPL_H

#include <QObject>
#include "confserverworker.h"

class ConfServerWorkerImpl : public ConfServerWorker
{
        Q_OBJECT
    public:
        explicit ConfServerWorkerImpl(QObject *parent = 0);

        QHash<int,DomainNodes*> * getStartConf();
        QList<ConnClient*> * getConnServer();
    signals:

    public slots:

    private:
        void init();
        void initParam();
};

#endif // CONFSERVERWORKERIMPL_H
