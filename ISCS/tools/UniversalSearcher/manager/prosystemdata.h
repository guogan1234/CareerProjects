#ifndef PROSYSTEMDATA_H
#define PROSYSTEMDATA_H

#include "data/searcher.h"
#include "confcmn.h"
#include <QList>

class ProSystemData
{
    public:
        ProSystemData();

        QList<ProSystem*> * loadDataByCFDB(QString sql);

    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<ProSystem*> * fillList(ConfDbRecordSet* record);
};

#endif // PROSYSTEMDATA_H
