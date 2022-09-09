#ifndef DEVINFO_H
#define DEVINFO_H

#include <QString>
#include "dev_struct.h"

struct DevNode{
    QString seq;
    int level;
    QString parent;
    QString sName;
    QString name;
    QList<QString> * childs;
};
typedef struct DevNode DevNode;

struct DevInfo{
    QList<process*> * proList;
    QList<cpu*> * cpuList;
    QList<mem*> * memList;
    QList<phy*> * phyList;
};
typedef struct DevInfo DevInfo;

#endif // DEVINFO_H
