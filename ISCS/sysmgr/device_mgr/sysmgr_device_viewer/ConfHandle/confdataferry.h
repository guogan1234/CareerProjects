#ifndef CONFDATAFERRY_H
#define CONFDATAFERRY_H

#include <QObject>
#include "confdata.h"

class ConfDataFerry : public QObject
{
    Q_OBJECT
public:
    explicit ConfDataFerry(QObject *parent = nullptr);

    int loadConfData();
    QList<ConnClient*> * getWorkPCConnClients();
    QList<ConnClient*> * getStationConnClients();
    QList<ConnClient*> * getLocalDomainServer();
    QHash<int,DomainNodes*> * getDomainNodes();
    QHash<int,DomainNodes*> * getLocalDomainNodes();
    signals:

public slots:

private:
    ConfData* confData;

    int init();
};

#endif // CONFDATAFERRY_H
