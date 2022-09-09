#include "cputablemodel.h"

CpuTableModel::CpuTableModel(QObject *parent) : QStandardItemModel(parent)
{

}

void CpuTableModel::setModelData(QList<Cpu *> *list)
{
    cpuList = list;
}

void CpuTableModel::start()
{
    clear();
    initModel();
}

void CpuTableModel::initModel()
{
    initHeaderLabels();
    initModelDatas();
}

void CpuTableModel::initHeaderLabels()
{
    QStringList headers;
    headers<<"CPU名称"<<"CPU使用率";
    setHorizontalHeaderLabels(headers);
}

void CpuTableModel::initModelDatas()
{
    if(!cpuList){
        qDebug("CpuTableModel:cpuList is NULL.");
        return;
    }
    for(int i = 0;i < cpuList->size(); ++i){
        Cpu* cpu = cpuList->at(i);
        if(!cpu){
            continue;
        }
        QStandardItem* nameItem = new QStandardItem(cpu->name);
        QStandardItem* usedItem = new QStandardItem(QString("%1 %2").arg(cpu->usedRate,0,'f',2).arg("%"));
        QList<QStandardItem*> list;
        list.append(nameItem);
        list.append(usedItem);

        appendRow(list);
    }
}
