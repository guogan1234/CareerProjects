#ifndef DEVTYPEDATA_H
#define DEVTYPEDATA_H

#include "data/searcher.h"
#include "confcmn.h"
#include <QList>

class DevTypeData
{
    public:
        DevTypeData();

        QList<DevType*> * loadDataByCFDB(QString sql);

    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<DevType*> * fillList(ConfDbRecordSet* record);
};

#endif // DEVTYPEDATA_H
