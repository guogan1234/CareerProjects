#ifndef AIDATA_H
#define AIDATA_H

#include "data/searcher.h"
#include "confcmn.h"
#include <QList>

class AiData
{
    public:
        AiData();

        QList<Ai*> * loadDataByCFDB(QString sql);
        char** getRecordHeaderByCFDB();
        int getRecordHeaderSizeByCFDB();
    private:
        ConfDbRecordSet* getRecordByCFDB(QString sql);
        QList<Ai*> * fillList(ConfDbRecordSet* record);

        char** columns;
        int size;
};

#endif // AIDATA_H
