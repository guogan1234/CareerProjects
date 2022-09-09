#ifndef DIACTIVEMODEL_H
#define DIACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/dimanager.h"

class DiActiveModel : public ActiveListModelBase
{
    Q_OBJECT
public:
    explicit DiActiveModel(QObject *parent = nullptr);

    QString conditionsToSQL(QVariantMap *listValues);
    void fillModelDatas(QString sql);

    char** getRecordHeaderByCFDB();
    int getRecordHeaderSizeByCFDB();

    Di* getModelDataByIndex(int index);
signals:

public slots:

private:
    DiManager* manager;

    void init();
    void initParam();

    QList<Di*> * getModelDatas(QString sql);
    void fillModel(QList<Di*> * data);

    QList<Di*> * modelData;
};

#endif // DIACTIVEMODEL_H
