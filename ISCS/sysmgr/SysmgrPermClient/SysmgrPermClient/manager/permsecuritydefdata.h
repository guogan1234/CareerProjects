#ifndef PERMSECURITYDEFDATA_H
#define PERMSECURITYDEFDATA_H

#include "data/permdefine.h"

class PermSecurityDefData
{
public:
    PermSecurityDefData();

    PermSecurityDef* getPermSecurityInfo(int *ret);
private:
    PermSecurityDef* getDataBySql(char* sql,int *ret);
};

#endif // PERMSECURITYDEFDATA_H
