#ifndef PERMTYPEDEFDATA_H
#define PERMTYPEDEFDATA_H

#include "data/permdefine.h"
#include <list>
using namespace std;

class PermTypeDefData
{
public:
    PermTypeDefData();

    char* getPermTypeNameById(int typeId);
private:
    void* loadDataBySql(char* sql);
};

#endif // PERMTYPEDEFDATA_H
