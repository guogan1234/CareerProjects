#ifndef PERMSECURITYDEFMANAGER_H
#define PERMSECURITYDEFMANAGER_H

#include <QObject>
#include "permsecuritydefdata.h"

class PermSecurityDefManager : public QObject
{
    Q_OBJECT
public:
    explicit PermSecurityDefManager(QObject *parent = 0);

    PermSecurityDef* getPermSecurityInfo(int *ret);
signals:

public slots:

private:
    PermSecurityDefData* data;
    void init();
    void initParam();
};

#endif // PERMSECURITYDEFMANAGER_H
