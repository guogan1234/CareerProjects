#ifndef PERMSECURITYDEFDATA_H
#define PERMSECURITYDEFDATA_H

#include "data/permdefine.h"
#include <QString>

class PermSecurityDefData
{
public:
    PermSecurityDefData();

    PermSecurityDef* getPermSecurityInfo(int *ret);
private:
    PermSecurityDef* getDataBySql(QString sql,int *ret);
};

#endif // PERMSECURITYDEFDATA_H
