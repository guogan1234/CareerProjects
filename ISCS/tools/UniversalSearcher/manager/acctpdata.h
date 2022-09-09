#ifndef ACCTPDATA_H
#define ACCTPDATA_H

#include "data/cfdbtable.h"
#include "confcmn.h"
#include <QList>

class AccTpData
{
    public:
        AccTpData();

        QList<AccTp*> * loadDataByCFDB(QString sql);

    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<AccTp*> * fillList(ConfDbRecordSet* record);
};

#endif // ACCTPDATA_H
