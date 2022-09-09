#ifndef DEVINFOREQUEST_H
#define DEVINFOREQUEST_H

#include <QString>
#include <QList>

struct DevInfoRequest{
    QString name;
    QList<QString> * ips;
};
typedef struct DevInfoRequest DevInfoRequest;

#endif // DEVINFOREQUEST_H
