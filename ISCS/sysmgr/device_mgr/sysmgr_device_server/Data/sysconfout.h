#ifndef SYSCONFOUT_H
#define SYSCONFOUT_H

#include <QList>
#include <QString>

struct ConnClient{
    QString name;
    QString name_ch;
    QList<QString> * ips;
};
typedef struct ConnClient ConnClient;

#endif // SYSCONFOUT_H
