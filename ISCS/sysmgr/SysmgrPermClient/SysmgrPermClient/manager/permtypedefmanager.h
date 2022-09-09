#ifndef PERMTYPEDEFMANAGER_H
#define PERMTYPEDEFMANAGER_H

#include "permtypedefdata.h"

class PermTypeDefManager
{
public:
    PermTypeDefManager();
    ~PermTypeDefManager();

    char* getPermTypeNameById(int typeId);
private:
    PermTypeDefData* data;
    void init();
    void initParam();
};

#endif // PERMTYPEDEFMANAGER_H
