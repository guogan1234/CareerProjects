#ifndef SYSCONFOUT_H
#define SYSCONFOUT_H

#include <QList>
#include <QString>

#include "sysconfin.h"

struct ConnClient{
    QString name;
    QString name_ch;
    QList<QString> * ips;
};
typedef struct ConnClient ConnClient;

struct DomainNodes{
    SysDomain* domain;//域配置信息
    QList<SysNode*> * nodes;//对应域的所有设备节点信息
};

#endif // SYSCONFOUT_H
