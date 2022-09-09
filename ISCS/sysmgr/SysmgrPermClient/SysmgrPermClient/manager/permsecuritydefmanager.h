#ifndef PERMSECURITYDEFMANAGER_H
#define PERMSECURITYDEFMANAGER_H

#include "permsecuritydefdata.h"

class PermSecurityDefManager
{
public:
    PermSecurityDefManager();
    ~PermSecurityDefManager();

    PermSecurityDef* getPermSecurityInfo(int *ret);
private:
    PermSecurityDefData* data;
    void init();
    void initParam();
};

#endif // PERMSECURITYDEFMANAGER_H
