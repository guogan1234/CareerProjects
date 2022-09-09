#ifndef NODESTREEMODEL_H
#define NODESTREEMODEL_H

#include <QStandardItemModel>
#include "Data/sysconfout.h"

class NodesTreeModel : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit NodesTreeModel(QObject *parent = 0);

        void setDevNodes(QHash<int,DomainNodes*> * configInfo);
        int start();
    signals:

    public slots:

    private:
        QHash<int,DomainNodes*> * devNodes;
        int initModel();
        void initHeaderLabels();
        int initModelDatas();
};

#endif // NODESTREEMODEL_H
