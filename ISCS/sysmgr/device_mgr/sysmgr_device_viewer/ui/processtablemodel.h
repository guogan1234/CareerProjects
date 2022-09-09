#ifndef PROCESSTABLEMODEL_H
#define PROCESSTABLEMODEL_H

#include <QStandardItemModel>
#include "Data/devinfo.h"

class ProcessTableModel : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit ProcessTableModel(QObject *parent = 0);

        void setModelData(QList<Process*> * list);
        void start();
    signals:

    public slots:

    private:
        QList<Process*> * processList;

        void initModel();
        void initHeaderLabels();
        void initModelDatas();
};

#endif // PROCESSTABLEMODEL_H
