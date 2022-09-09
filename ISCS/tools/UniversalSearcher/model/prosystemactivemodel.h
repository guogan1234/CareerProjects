#ifndef PROSYSTEMACTIVEMODEL_H
#define PROSYSTEMACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/prosystemmanager.h"

class ProSystemActiveModel : public ActiveListModelBase
{
    Q_OBJECT
public:
    explicit ProSystemActiveModel(QObject *parent = nullptr);
    ~ProSystemActiveModel();

    QString conditionsToSQL(QVariantMap *listValues);
    void fillModelDatas(QString sql);
    void initShow();
signals:

public slots:

private:
    QList<ProSystem*> * getModelDatas(QString sql);
    void fillModel(QList<ProSystem*> * data);

    ProSystemManager* manager;

    void init();
    void initParam();
};

#endif // PROSYSTEMACTIVEMODEL_H
