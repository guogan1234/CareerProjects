#include "nettablemodel.h"

NetTableModel::NetTableModel(QObject *parent) : QStandardItemModel(parent)
{

}

void NetTableModel::setModelData(NetInfo *info)
{
    netInfo = info;
}

void NetTableModel::start()
{
    clear();
    initModel();
}

void NetTableModel::initModel()
{
    initHeaderLabels();
    initModelDatas();
}

void NetTableModel::initHeaderLabels()
{
    QStringList headers;
    headers<<"设备编号"<<"设备ip1"<<"连接状态"<<"设备ip2"<<"连接状态"<<"总连接状态";
    setHorizontalHeaderLabels(headers);
}

void NetTableModel::initModelDatas()
{
    if(!netInfo){
        qDebug("NetTableModel:netInfo is NULL.");
        return;
    }
    if(netInfo->netConn){
        QStandardItem* nameItem = new QStandardItem(netInfo->name);
        QList<QStandardItem*> list;
        list.append(nameItem);

        QString bConnTotal = "未连接";
        int len = netInfo->netConn->size();
        for(int i = 0;i < len;i++){
            NetConn* temp = netInfo->netConn->at(i);

            QString bConn;
            if(temp->bConn){
                bConn = "已连接";
                bConnTotal = "已连接";
            }else{
                bConn = "未连接";
            }    
            QStandardItem* ipItem = new QStandardItem(temp->ip);
            QStandardItem* connItem = new QStandardItem(bConn);

            list.append(ipItem);
            list.append(connItem);       
        }
        QStandardItem* connTotalItem = new QStandardItem(bConnTotal);
        list.append(connTotalItem);
        appendRow(list);
    }
}
