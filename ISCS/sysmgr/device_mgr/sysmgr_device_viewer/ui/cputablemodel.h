#ifndef CPUTABLEMODEL_H
#define CPUTABLEMODEL_H

#include <QStandardItemModel>
#include "Data/devinfo.h"

class CpuTableModel : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit CpuTableModel(QObject *parent = 0);

        void setModelData(QList<Cpu*> * list);
        void start();
    signals:

    public slots:

    private:
        QList<Cpu*> * cpuList;

        void initModel();
        void initHeaderLabels();
        void initModelDatas();
};

#endif // CPUTABLEMODEL_H
