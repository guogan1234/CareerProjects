#ifndef DIDATA_H
#define DIDATA_H

#include "data/searcher.h"
#include "confcmn.h"
#include <QList>

class DiData
{
    public:
        DiData();

        QList<Di*> * loadDataByCFDB(QString sql);
        char** getRecordHeaderByCFDB();
        int getRecordHeaderSizeByCFDB();
    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<Di*> * fillList(ConfDbRecordSet* record);

        char** columns;
        int size;
};

#endif // DIDATA_H
