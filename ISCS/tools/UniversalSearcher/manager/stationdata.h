#ifndef STATIONDATA_H
#define STATIONDATA_H

#include "data/searcher.h"
#include "confcmn.h"
#include <QList>

class StationData
{
public:
    StationData();

    QList<Station*> * loadDataByCFDB(QString sql);

private:
    ConfDbRecordSet* getRecordByCFDB(QString sql);
    QList<Station*> * fillList(ConfDbRecordSet* record);
};

#endif // STATIONDATA_H
