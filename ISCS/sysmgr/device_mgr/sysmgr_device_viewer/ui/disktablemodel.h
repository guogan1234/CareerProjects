#ifndef DISKTABLEMODEL_H
#define DISKTABLEMODEL_H

#include <QStandardItemModel>
#include "Data/devinfo.h"

class DiskTableModel : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit DiskTableModel(QObject *parent = 0);

        void setModelData(QList<Disk*> * list);
        void start();
    signals:

    public slots:

    private:
        QList<Disk*> * diskList;

        void initModel();
        void initHeaderLabels();
        void initModelDatas();
};

#endif // DISKTABLEMODEL_H
