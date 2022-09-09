#ifndef PERMFUNCDEFDATA_H
#define PERMFUNCDEFDATA_H

#include "data/permdefine.h"
#include <list>
using namespace std;

class PermFuncDefData
{
public:
    PermFuncDefData();

    list<PermFuncDef*> * getInfoByPermIdAndPermType(list<int> * permIds,char* typeName,int *ret);
    list<PermFuncDef*> * getInfoByPermIdAndPermTypeId(list<int> * permIds,int permTypeId,int *ret);
    list<PermFuncDef*> * getInfoByPermIds(list<int> * permIds);

    void deleteInfoList(list<PermFuncDef*> * datas);
private:
    list<PermFuncDef*> * loadDataBySql(char* sql,int *ret);
};

#endif // PERMFUNCDEFDATA_H
