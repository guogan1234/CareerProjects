#ifndef ACCDEFDATA_H
#define ACCDEFDATA_H

#include "data/cfdbtable.h"
#include "confcmn.h"
#include <QList>

class AccDefData
{
    public:
        AccDefData();

        QList<AccDef*> * loadDataByCFDB(QString sql);

    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<AccDef*> * fillList(ConfDbRecordSet* record);
};

#endif // ACCDEFDATA_H
