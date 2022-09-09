#include "confdata.h"
#include <QDebug>

ConfData::ConfData(QObject *parent) : QObject(parent)
{
    init();
}

int ConfData::loadData()
{
    initData();

    return 0;
}

QList<ConnClient *> *ConfData::getWorkPCConnClients()
{
    QList<ConnClient*> * ret = NULL;
    ret = takeWorkPCConnClients();

    return ret;
}

QList<ConnClient *> *ConfData::getStationConnClients()
{
    QList<ConnClient*> * ret = NULL;
    ret = takeStationConnClients();

    return ret;
}

QList<ConnClient *> *ConfData::getLocalDomainServer()
{
    return takeGetLocalDomainServer();
}

QHash<int, DomainNodes *> *ConfData::getDomainNodes()
{
    return takeGetDomainNodes();
}

QHash<int, DomainNodes *> *ConfData::getLocalDomainNodes()
{
    return takeGetLocalDomainNodes();
}

int ConfData::init()
{
    initParam();

    return 0;
}

int ConfData::initParam()
{
    sysDomainList = new QList<SysDomain*>();
    sysNodeList = new QList<SysNode*>();
    sysProcessList = new QList<SysProcess*>();
    sysDomainProcList = new QList<SysDomainProc*>();

    return 0;
}

int ConfData::initData()
{
    qDebug("ConfData::initData...");
    sys_conf_system_manager_domain* data = sys_conf_get_system_manager_domain();
    if(!data) return -1;
    sys_conf_system_node* node = data->node_list_first;
    while(node){
        SysNode* sysNode = new SysNode();
        sysNode->type = node->type;
        sysNode->name = node->name;
        sysNode->name_ch = node->name_ch;
        sysNode->status = node->status;
        sysNode->nic_num = node->nic_num;
        sysNode->nic_1_name = node->nic_1_name;
        sysNode->nic_1_netmask = node->nic_1_netmask;
        sysNode->nic_1_address = node->nic_1_address;
        sysNode->nic_2_name = node->nic_2_name;
        sysNode->nic_2_netmask = node->nic_2_netmask;
        sysNode->nic_2_address = node->nic_2_address;
        sysNode->net_status = node->net_status;
        sysNode->dev = node->dev;
        sysNode->os = node->os;
        sysNode->location = node->location;
        sysNode->domain_id = node->domain_id;
//        sysNode->domain_id = 1;

        node = node->next;
        sysNodeList->append(sysNode);
    }

    sys_conf_system_domain* domain = data->domain_list_first;
    while(domain){
        SysDomain* sysDomain = new SysDomain();
        sysDomain->name = domain->name;
        sysDomain->name_ch = domain->name_ch;
        sysDomain->domain_type = domain->domain_type;
//        sysDomain->domain_id = 1;//配置接口暂无
        sysDomain->domain_id = domain->id;

        sysDomainList->append(sysDomain);
        domain = domain->next;
    }

    return 0;
}

QList<ConnClient *> *ConfData::takeWorkPCConnClients()
{
    QList<ConnClient*> * ret = new QList<ConnClient*>();
    foreach (SysNode* temp, *sysNodeList) {
        if(3 == temp->type){
            ConnClient* connClient = new ConnClient();
            connClient->name = temp->name;
            connClient->name_ch = temp->name_ch;
            connClient->ips = new QList<QString>();
            connClient->ips->append(QString(temp->nic_1_address));
            connClient->ips->append(QString(temp->nic_2_address));

            ret->append(connClient);
        }
    }

    return ret;
}

QList<ConnClient *> *ConfData::takeStationConnClients()
{
    QList<ConnClient*> * ret = new QList<ConnClient*>();
    foreach (SysNode* temp, *sysNodeList) {
        if(1 == temp->type){
            ConnClient* connClient = new ConnClient();
            connClient->name = temp->name;
            connClient->name_ch = temp->name_ch;
            connClient->ips = new QList<QString>();
            connClient->ips->append(QString(temp->nic_1_address));
            connClient->ips->append(QString(temp->nic_2_address));

            ret->append(connClient);
        }
    }

    return ret;
}

//
QList<ConnClient *> *ConfData::takeGetLocalDomainServer()
{
    if(sysNodeList->isEmpty()) return NULL;
    int localDomainId = getLocalDomainId();
    if(-1 == localDomainId) return NULL;
    QList<ConnClient*> * ret = NULL;
    for(int i = 0;i < sysNodeList->size(); ++i){
        SysNode* node = sysNodeList->at(i);
        //获取本域所有节点的域服务器连接信息IP
        if(node && node->domain_id == localDomainId && node->type == 1){
            ConnClient* client = new ConnClient();
            client->name.append(node->name);
            client->name_ch.append(node->name_ch);
            QString ip_1;
            ip_1.append(node->nic_1_address);
            QString ip_2;
            ip_2.append(node->nic_2_address);
            QList<QString> * ips = new QList<QString>();
            ips->append(ip_1);
            ips->append(ip_2);
            client->ips = ips;

            ret = new QList<ConnClient*>();
            ret->append(client);
            return ret;
        }
    }
    return ret;
}

QHash<int, DomainNodes *> *ConfData::takeGetLocalDomainNodes()
{
    if(sysDomainList->isEmpty())    return NULL;
    //获取本地域编号
    int localDomainId = getLocalDomainId();
    if(-1 == localDomainId) return NULL;
    for(int i = 0;i < sysDomainList->size(); ++i){
        SysDomain* domain = sysDomainList->at(i);
        if(domain){
            if(localDomainId == domain->domain_id){
                //本地域若为中心站，获取所有节点；否则，只获取本地节点
                qDebug()<<"local domain type -- "<<domain->domain_type;
                if(domain->domain_type == 1)//center
                {
                    return takeGetDomainNodes();
                }else {
                    QList<SysNode*> * nodeList = NULL;
                    nodeList = getDomainSysNodes(localDomainId);
                    DomainNodes* domainNode = new DomainNodes();
                    domainNode->domain = domain;
                    domainNode->nodes = nodeList;
                    QHash<int,DomainNodes*> * ret = new QHash<int,DomainNodes*>();
                    ret->insertMulti(localDomainId,domainNode);
                    return ret;
                }
            }
        }
    }
    return NULL;
}

int ConfData::getLocalDomainId()
{
//    int domainId = 1;
    int domainId = -1;
    domainId = sys_conf_get_local_domain_id(&domainId);
    return domainId;
}

//获取所有域和域下节点
QHash<int, DomainNodes *> *ConfData::takeGetDomainNodes()
{
    if(sysDomainList->isEmpty())    return NULL;
    QHash<int,DomainNodes*> * ret = new QHash<int,DomainNodes*>();
    for(int i = 0;i < sysDomainList->size(); ++i){
        SysDomain* domain = sysDomainList->at(i);
        QList<SysNode*> * nodeList = NULL;
        int domainId = -1;
        if(domain){
            domainId = domain->domain_id;
            nodeList = getDomainSysNodes(domainId);
        }
        DomainNodes* domainNodes = new DomainNodes();
        domainNodes->domain = domain;
        domainNodes->nodes = nodeList;
        qDebug()<<"domainId - "<<domainId<<"nodeList size - "<<nodeList->size();

        ret->insertMulti(domainId,domainNodes);
    }
    return ret;
}

QList<SysNode *> *ConfData::getDomainSysNodes(int domainId)
{
    if(sysNodeList->isEmpty()) return NULL;
    QList<SysNode*> * ret = new QList<SysNode*>();
    for(int i = 0;i < sysNodeList->size(); ++i){
        SysNode* node = sysNodeList->at(i);
        if(node){
            if(node->domain_id == domainId){
                ret->append(node);
            }
        }
    }
    return ret;
}
