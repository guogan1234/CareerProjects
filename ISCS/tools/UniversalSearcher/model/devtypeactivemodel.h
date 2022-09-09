#ifndef DEVTYPEACTIVEMODEL_H
#define DEVTYPEACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/devtypemanager.h"

class DevTypeActiveModel : public ActiveListModelBase
{
    Q_OBJECT
public:
    explicit DevTypeActiveModel(QObject *parent = nullptr);
    ~DevTypeActiveModel();

    QString conditionsToSQL(QVariantMap *listValues);
    void fillModelDatas(QString sql);
    void initShow();
signals:

public slots:

private:
    QList<DevType*> * getModelDatas(QString sql);
    void fillModel(QList<DevType*> * data);

    DevTypeManager* manager;

    void init();
    void initParam();
};

#endif // DEVTYPEACTIVEMODEL_H
