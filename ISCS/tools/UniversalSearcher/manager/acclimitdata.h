#ifndef ACCLIMITDATA_H
#define ACCLIMITDATA_H

#include "data/cfdbtable.h"
#include "confcmn.h"
#include <QList>

class AccLimitData
{
    public:
        AccLimitData();

        QList<AccLimit*> * loadDataByCFDB(QString sql);

    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<AccLimit*> * fillList(ConfDbRecordSet* record);
};

#endif // ACCLIMITDATA_H
