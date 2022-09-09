#include "disktablemodel.h"
#include "Common/bytecaster.h"

DiskTableModel::DiskTableModel(QObject *parent) : QStandardItemModel(parent)
{

}

void DiskTableModel::setModelData(QList<Disk *> *list)
{
    diskList = list;
}

void DiskTableModel::start()
{
    clear();
    initModel();
}

void DiskTableModel::initModel()
{
    initHeaderLabels();
    initModelDatas();
}

void DiskTableModel::initHeaderLabels()
{
    QStringList headers;
    headers<<"磁盘名称"<<"总量"<<"已使用量"<<"空余量"<<"磁盘使用率";
    setHorizontalHeaderLabels(headers);
}

void DiskTableModel::initModelDatas()
{
    if(!diskList){
        qDebug("DiskTableModel:diskList is NULL.");
        return;
    }
    for(int i = 0;i < diskList->size(); ++i){
        Disk* disk = diskList->at(i);
        if(!disk){
            continue;
        }
        QStandardItem* nameItem = new QStandardItem(disk->name);
        QString total = ByteCaster::bytesToGB(disk->total);
        QString used = ByteCaster::bytesToGB(disk->used);
        QString free = ByteCaster::bytesToGB(disk->free);
        QStandardItem* totalItem = new QStandardItem(total);
        QStandardItem* usedItem = new QStandardItem(used);
        QStandardItem* freeItem = new QStandardItem(free);
        QString temp = QString::number(disk->usedRate);
        QStringList strList = temp.split('.');
        if(strList.size() <= 0) return;
        QStandardItem* rateItem = new QStandardItem(QString("%1 %2").arg(strList.value(0)).arg('%'));
//        QStandardItem* rateItem = new QStandardItem(QString("%1 %2").arg(disk->usedRate*100,4).arg("%"));
        QList<QStandardItem*> list;
        list.append(nameItem);
        list.append(totalItem);
        list.append(usedItem);
        list.append(freeItem);
        list.append(rateItem);

        appendRow(list);
    }
}
