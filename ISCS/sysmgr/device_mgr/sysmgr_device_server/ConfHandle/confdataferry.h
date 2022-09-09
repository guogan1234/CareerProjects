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
    QList<ConnClient*> * getLocalDomainConnClients();
    AlarmExtra* getAlarmExtraByIp(QString ip);
signals:

public slots:

private:
    ConfData* confData;

    int init();
};

#endif // CONFDATAFERRY_H
