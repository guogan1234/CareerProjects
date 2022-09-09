#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include <QObject>
#include "Data/sysconfout.h"

class NodeManager : public QObject
{
        Q_OBJECT
    public:
        explicit NodeManager(QObject *parent = 0);

        void setDomainNodes(QHash<int,DomainNodes*> * hash);
    signals:

    public slots:

    private:
        void init();
        void initParam();

        QHash<int,DomainNodes*> * domainNodes;
};

#endif // NODEMANAGER_H
