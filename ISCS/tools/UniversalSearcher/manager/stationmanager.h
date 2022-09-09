#ifndef STATIONMANAGER_H
#define STATIONMANAGER_H

#include <QObject>
#include "stationdata.h"

class StationManager : public QObject
{
        Q_OBJECT
    public:
        explicit StationManager(QObject *parent = 0);

        QList<Station*> * loadDataByCFDB(QString sql);
    signals:

    public slots:

    private:
        StationData* data;

        void init();
        void initParam();
};

#endif // STATIONMANAGER_H
