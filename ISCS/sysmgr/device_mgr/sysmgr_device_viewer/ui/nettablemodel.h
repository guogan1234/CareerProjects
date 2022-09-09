#ifndef NETTABLEMODEL_H
#define NETTABLEMODEL_H

#include <QStandardItemModel>
#include "Data/netinfo.h"

class NetTableModel : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit NetTableModel(QObject *parent = 0);

        void setModelData(NetInfo* info);
        void start();
    signals:

    public slots:

    private:
        NetInfo* netInfo;

        void initModel();
        void initHeaderLabels();
        void initModelDatas();
};

#endif // NETTABLEMODEL_H
