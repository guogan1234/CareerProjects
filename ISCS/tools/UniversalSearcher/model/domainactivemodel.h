#ifndef DOMAINACTIVEMODEL_H
#define DOMAINACTIVEMODEL_H

#include <QObject>
#include "activelistmodelbase.h"
#include "manager/domainmanager.h"

class DomainActiveModel : public ActiveListModelBase
{
        Q_OBJECT
    public:
        explicit DomainActiveModel(QObject *parent = 0);
        ~DomainActiveModel();

        QString conditionsToSQL(QVariantMap *listValues);
        void fillModelDatas(QString sql);
        void initShow();
    signals:

    public slots:

    private:
        QList<Domain*> * getModelDatas(QString sql);
        void fillModel(QList<Domain*> * data);

        DomainManager* manager;

        void init();
        void initParam();
};

#endif // DOMAINACTIVEMODEL_H
