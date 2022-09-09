#include "confdata.h"

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

QList<ConnClient *> *ConfData::getLocalDomainConnClients()
{
    return takeGetLocalDomainConnClients();
}

AlarmExtra *ConfData::getAlarmExtraByIp(QString ip)
{
    QByteArray bArray = ip.toLocal8Bit();
    char* nodeIp = bArray.data();
    foreach (SysNode* temp, *sysNodeList) {
        if(!strcmp(nodeIp,temp->nic_1_address) || !strcmp(nodeIp,temp->nic_2_address)){
            AlarmExtra* ret = new AlarmExtra();
            ret->domainId = temp->domain_id;
            ret->stationId = -1;
            ret->nodeName = strdup(temp->name);

            return ret;
        }
    }
    return NULL;
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

QList<ConnClient *> *ConfData::takeGetLocalDomainConnClients()
{
    int localDomainId = getLocalDomainId();
    if(-1 == localDomainId) return NULL;
    QList<ConnClient*> * ret = new QList<ConnClient*>();
    foreach (SysNode* temp, *sysNodeList) {
        if(localDomainId == temp->domain_id){
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

int ConfData::getLocalDomainId()
{
//    int domainId = 1;
    int domainId = -1;
    domainId = sys_conf_get_local_domain_id(&domainId);
    return domainId;
}
