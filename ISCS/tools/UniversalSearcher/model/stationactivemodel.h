#ifndef STATIONACTIVEMODEL_H
#define STATIONACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/stationmanager.h"

class StationActiveModel : public ActiveListModelBase
{
        Q_OBJECT
    public:
        explicit StationActiveModel(QObject *parent = 0);
        ~StationActiveModel();

        QString conditionsToSQL(QVariantMap *listValues);
        void fillModelDatas(QString sql);
    signals:

    public slots:

    private:
        StationManager* manager;

        void init();
        void initParam();

        QList<Station*> * getModelDatas(QString sql);
        void fillModel(QList<Station*> * data);
};

#endif // STATIONACTIVEMODEL_H
