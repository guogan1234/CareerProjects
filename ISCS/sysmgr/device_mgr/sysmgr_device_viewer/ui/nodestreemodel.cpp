#include "nodestreemodel.h"
#include <QDebug>

NodesTreeModel::NodesTreeModel(QObject *parent) : QStandardItemModel(parent)
{

}

void NodesTreeModel::setDevNodes(QHash<int, DomainNodes *> *configInfo)
{
    devNodes = configInfo;
}

int NodesTreeModel::start()
{
    return initModel();
}

int NodesTreeModel::initModel()
{
    initHeaderLabels();
    initModelDatas();

    return 0;
}

void NodesTreeModel::initHeaderLabels()
{
    QStringList headers;
//    headers<<"设备列表"<<"设备编号";
    headers<<"设备列表";
    setHorizontalHeaderLabels(headers);
}

int NodesTreeModel::initModelDatas()
{
    if(!devNodes) return -1;
    QStandardItem* root = invisibleRootItem();
    QHash<int, DomainNodes*>::const_iterator i = devNodes->constBegin();
    while (i != devNodes->constEnd()) {
        qDebug() << i.key() << ": " << i.value();
        DomainNodes* domainNode = i.value();
        SysDomain* domain = NULL;
        QList<SysNode*> * nodeList = NULL;
        if(domainNode){
            domain = domainNode->domain;
            nodeList = domainNode->nodes;
        }
        if(domain){
            QString domainName;
            domainName.append(domain->name_ch);
            QString domainId;
            domainId = QString("%1").arg(domain->domain_id);
            QStandardItem* domainNameItem = new QStandardItem(domainName);
//            QStandardItem* domainIdItem = new QStandardItem(domainId);
            if(nodeList){
                for(int i = 0;i < nodeList->size(); ++i){
                    SysNode* node = nodeList->at(i);
                    if(node){
                        QString nodeName;
                        nodeName.append(node->name_ch);
                        QString nodeType;
                        nodeType = QString("%1").arg(node->type);
                        QStandardItem* nodeNameItem = new QStandardItem(nodeName);
                        QVariantMap itemDataMap;
                        itemDataMap.insert("name",QVariant(node->name));
                        //添加节点附加信息
                        QVariantList dataIpList;
                        QString ip_1(node->nic_1_address);
                        QString ip_2(node->nic_2_address);
                        dataIpList.append(QVariant(ip_1));
                        dataIpList.append(QVariant(ip_2));
                        itemDataMap.insert("ips",QVariant(dataIpList));
                        QVariant nodeData(itemDataMap);
                        nodeNameItem->setData(nodeData,Qt::UserRole);

//                        QStandardItem* nodeTypeItem = new QStandardItem(nodeType);
                        QList<QStandardItem*> nodeItemsList;
                        nodeItemsList.append(nodeNameItem);
//                        nodeItemsList.append(nodeTypeItem);
                        domainNameItem->appendRow(nodeItemsList);
//                        domainIdItem->appendRow(nodeItemsList);
                    }
                }
            }

            QList<QStandardItem*> list;
            list.append(domainNameItem);
//            list.append(domainIdItem);

            root->appendRow(list);
        }
        ++i;
    }

    return 0;
}
