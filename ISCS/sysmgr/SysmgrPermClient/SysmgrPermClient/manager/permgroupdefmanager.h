#ifndef PERMGROUPDEFMANAGER_H
#define PERMGROUPDEFMANAGER_H

#include "permgroupdefdata.h"

class PermGroupDefManager
{
public:
    PermGroupDefManager();
    ~PermGroupDefManager();

    list<PermGroupDef*> * loadAll(int *ret);
    list<PermGroupDef*> * loadGroupInfoById(int groupId,int* ret);
    list<PermGroupDef*> * loadGroupInfoByName(const char* groupName,int* ret);
    void releaseGroupInfo(list<PermGroupDef*> * datas);

private:
    PermGroupDefData* data;
    void init();
    void initParam();
};

#endif // PERMGROUPDEFMANAGER_H
