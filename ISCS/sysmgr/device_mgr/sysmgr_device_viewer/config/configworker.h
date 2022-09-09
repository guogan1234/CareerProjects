#ifndef CONFIGWORKER_H
#define CONFIGWORKER_H

#include <QObject>
#include "Data/keyvalue.h"
#include "Data/sysconfout.h"

class ConfigWorker : public QObject
{
    Q_OBJECT
public:
    explicit ConfigWorker(QObject *parent = 0);

    void setConfig(QList<KeyValue*> * configData);
    void setConnectInfo(QList<ConnClient*> * info);
signals:

public slots:
    void handOutConfigSlot();
private:
    QList<KeyValue*> * config;
    QList<ConnClient*> * connInfo;
};

#endif // CONFIGWORKER_H
