#ifndef DOMAINMANAGER_H
#define DOMAINMANAGER_H

#include <QObject>
#include "domaindata.h"

class DomainManager : public QObject
{
    Q_OBJECT
public:
    explicit DomainManager(QObject *parent = nullptr);

    QList<Domain*> * loadDataByCFDB(QString sql);
signals:

public slots:

private:
    DomainData* data;

    void init();
    void initParam();
};

#endif // DOMAINMANAGER_H
