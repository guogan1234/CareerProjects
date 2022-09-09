#ifndef PERMGROUPDEFDATA_H
#define PERMGROUPDEFDATA_H

#include "data/permdefine.h"
#include <list>
using namespace std;

class PermGroupDefData
{
public:
    PermGroupDefData();

    list<PermGroupDef*> * loadAll(int *ret);
    list<PermGroupDef*> * loadGroupInfoById(int groupId,int* ret);
    list<PermGroupDef*> * loadGroupInfoByName(const char* groupName,int* ret);
    void releaseGroupInfo(list<PermGroupDef*> * datas);

  private:
    list<PermGroupDef*> * loadDataBySql(char* sql,int *ret);
};

#endif // PERMGROUPDEFDATA_H
