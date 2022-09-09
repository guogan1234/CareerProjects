#ifndef DEVICEACTIVEMODEL_H
#define DEVICEACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/devmanager.h"

class DeviceActiveModel : public ActiveListModelBase
{
    Q_OBJECT
public:
    explicit DeviceActiveModel(QObject *parent = nullptr);

    QString conditionsToSQL(QVariantMap *listValues);
    void fillModelDatas(QString sql);
signals:

public slots:

private:
    DevManager* manager;

    void init();
    void initParam();

    QList<Dev*> * getModelDatas(QString sql);
    void fillModel(QList<Dev*> * data);
};

#endif // DEVICEACTIVEMODEL_H
