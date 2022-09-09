#ifndef GENERALDATA_H
#define GENERALDATA_H

#include <QList>
#include "confcmn.h"

class GeneralData
{
public:
    GeneralData();

    QList<QVariantMap*> * loadDataByCFDB(QString sql);

private:
    ConfDbRecordSet* getRecordByCFDB(QString sql);
    QList<QVariantMap*> * fillList(ConfDbRecordSet* record);
};

#endif // GENERALDATA_H
