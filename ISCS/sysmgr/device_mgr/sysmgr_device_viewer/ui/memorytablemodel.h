#ifndef MEMORYTABLEMODEL_H
#define MEMORYTABLEMODEL_H

#include <QStandardItemModel>
#include "Data/devinfo.h"

class MemoryTableModel : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit MemoryTableModel(QObject *parent = 0);

        void setModelData(Memory* data);
        void start();
    signals:

    public slots:

    private:
        Memory* memory;

        void initModel();
        void initHeaderLabels();
        void initModelDatas();
};

#endif // MEMORYTABLEMODEL_H
