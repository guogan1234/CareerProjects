#ifndef ACTIVELISTMODELBASE_H
#define ACTIVELISTMODELBASE_H

#include <QStandardItemModel>

class ActiveListModelBase : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit ActiveListModelBase(QObject *parent = 0);

        virtual QString conditionToSQL(QList<QString> * list) = 0;
        virtual void fillModelDatas(QString sql) = 0;
    signals:

    public slots:
};

#endif // ACTIVELISTMODELBASE_H
