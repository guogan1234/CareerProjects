#ifndef DOMAINDATA_H
#define DOMAINDATA_H

#include "data/domain.h"
#include "confcmn.h"
#include <QList>

class DomainData
{
public:
    DomainData();

    QList<Domain*> * loadDataByCFDB(QString sql);

private:
    QList<Domain*> * fillList(ConfDbRecordSet* record);
};

#endif // DOMAINDATA_H
