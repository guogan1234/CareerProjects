#ifndef DEVDATA_H
#define DEVDATA_H

#include "data/searcher.h"
#include "confcmn.h"
#include <QList>

class DevData
{
    public:
        DevData();

        QList<Dev*> * loadDataByCFDB(QString sql);

    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<Dev*> * fillList(ConfDbRecordSet* record);
};

#endif // DEVDATA_H
