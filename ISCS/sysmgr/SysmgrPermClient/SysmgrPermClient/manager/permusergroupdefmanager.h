#ifndef PERMUSERGROUPDEFMANAGER_H
#define PERMUSERGROUPDEFMANAGER_H

#include "permusergroupdefdata.h"

class PermUserGroupDefManager
{
public:
    PermUserGroupDefManager();
    ~PermUserGroupDefManager();

    list<int> * getGroupIdsByUserId(int userId);
    int delUserGroupDefByUserId(int userId);
private:
    PermUserGroupDefData* data;
    void init();
    void initParam();
};

#endif // PERMUSERGROUPDEFMANAGER_H
