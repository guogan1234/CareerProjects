#ifndef ACCACTIVEMODEL_H
#define ACCACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/accmanager.h"

class AccActiveModel : public ActiveListModelBase
{
    Q_OBJECT
public:
    explicit AccActiveModel(QObject *parent = nullptr);

    QString conditionsToSQL(QVariantMap *listValues);
    void fillModelDatas(QString sql);

    char** getRecordHeaderByCFDB();
    int getRecordHeaderSizeByCFDB();

    Acc* getModelDataByIndex(int index);
signals:

public slots:

private:
    AccManager* manager;

    void init();
    void initParam();

    QList<Acc*> * getModelDatas(QString sql);
    void fillModel(QList<Acc*> * data);

    QList<Acc*> * modelData;
};

#endif // ACCACTIVEMODEL_H
