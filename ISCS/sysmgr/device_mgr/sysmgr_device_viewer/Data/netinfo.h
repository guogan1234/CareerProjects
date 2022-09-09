#ifndef NETINFO_H
#define NETINFO_H

#include <QString>

struct NetConn{
    QString ip;
    bool bConn;
};
typedef struct NetConn NetConn;

struct NetInfo{
    QString name;
//    bool bConn;
    QList<NetConn*> * netConn;
};
typedef struct NetInfo NetInfo;

#endif // NETINFO_H
