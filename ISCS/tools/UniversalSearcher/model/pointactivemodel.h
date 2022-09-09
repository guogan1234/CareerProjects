#ifndef POINTACTIVEMODEL_H
#define POINTACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/accmanager.h"

class PointActiveModel : public ActiveListModelBase
{
    Q_OBJECT
public:
    explicit PointActiveModel(QObject *parent = nullptr);

    QString conditionsToSQL(QVariantMap *listValues);
    void fillModelDatas(QString sql);
signals:

public slots:
};

#endif // POINTACTIVEMODEL_H
