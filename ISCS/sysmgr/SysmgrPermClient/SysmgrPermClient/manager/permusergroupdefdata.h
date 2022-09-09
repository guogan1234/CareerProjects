#ifndef PERMUSERGROUPDEFDATA_H
#define PERMUSERGROUPDEFDATA_H

#include <list>
using namespace std;

class PermUserGroupDefData
{
public:
    PermUserGroupDefData();

    list<int> * getGroupIdsByUserId(int userId);
    int delUserGroupDefByUserId(int userId);
private:
    int simpleExecSql(const char* sql,int *ret);
};

#endif // PERMUSERGROUPDEFDATA_H
