#include "memorytablemodel.h"
#include "Common/bytecaster.h"

MemoryTableModel::MemoryTableModel(QObject *parent) : QStandardItemModel(parent)
{

}

void MemoryTableModel::setModelData(Memory *data)
{
    memory = data;
}

void MemoryTableModel::start()
{
    clear();
    initModel();
}

void MemoryTableModel::initModel()
{
    initHeaderLabels();
    initModelDatas();
}

void MemoryTableModel::initHeaderLabels()
{
    QStringList headers;
    headers<<"内存总量"<<"内存使用量"<<"内存空余量";
    setHorizontalHeaderLabels(headers);
}

void MemoryTableModel::initModelDatas()
{
    if(!memory){
        qDebug("MemoryTableModel:memory is NULL.");
        return;
    }
    QString total = ByteCaster::bytesToGB(memory->total);
    QString used = ByteCaster::bytesToGB(memory->used);
    QString free = ByteCaster::bytesToGB(memory->free);
    QStandardItem* totalItem = new QStandardItem(total);
    QStandardItem* usedItem = new QStandardItem(used);
    QStandardItem* freeItem = new QStandardItem(free);
    QList<QStandardItem*> list;
    list.append(totalItem);
    list.append(usedItem);
    list.append(freeItem);

    appendRow(list);
}
