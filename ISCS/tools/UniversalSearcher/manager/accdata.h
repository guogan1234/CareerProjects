#ifndef ACCDATA_H
#define ACCDATA_H

#include "data/searcher.h"
#include "confcmn.h"
#include <QList>

class AccData
{
    public:
        AccData();

        QList<Acc*> * loadDataByCFDB(QString sql);
        char** getRecordHeaderByCFDB();
        int getRecordHeaderSizeByCFDB();
    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<Acc*> * fillList(ConfDbRecordSet* record);

        char** columns;
        int size;
};

#endif // ACCDATA_H
