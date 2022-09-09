#include "processtablemodel.h"
#include "Common/bytecaster.h"

ProcessTableModel::ProcessTableModel(QObject *parent) : QStandardItemModel(parent)
{

}

void ProcessTableModel::setModelData(QList<Process *> *list)
{
    processList = list;
}

void ProcessTableModel::start()
{
    clear();
    initModel();
}

void ProcessTableModel::initModel()
{
    initHeaderLabels();
    initModelDatas();
}

void ProcessTableModel::initHeaderLabels()
{
    QStringList headers;
    headers<<"进程名"<<"CPU使用率"<<"内存";
//    headers<<"进程名"<<"CPU使用率"<<"内存"<<"正在运行";
    setHorizontalHeaderLabels(headers);
}

void ProcessTableModel::initModelDatas()
{
    if(!processList){
        qDebug("ProcessTableModel:processList is NULL.");
        return;
    }
    for(int i = 0;i < processList->size(); ++i){
        Process* process = processList->at(i);
        if(!process){
            continue;
        }
        QStandardItem* nameItem = new QStandardItem(process->name);
        QStandardItem* cpuItem = new QStandardItem(QString::number(process->cpuRate));
        QString memUsed = ByteCaster::bytesToKB(process->memUsed);
        QStandardItem* memItem = new QStandardItem(memUsed);
//        QStandardItem* runItem = new QStandardItem(QString::number(process->bRun));
        QList<QStandardItem*> list;
        list.append(nameItem);
        list.append(cpuItem);
        list.append(memItem);
//        list.append(runItem);

        appendRow(list);
    }
}
