#ifndef PERMFUNCDEFMANAGER_H
#define PERMFUNCDEFMANAGER_H

#include "permfuncdefdata.h"

class PermFuncDefManager
{
public:
    PermFuncDefManager();
    ~PermFuncDefManager();

    list<PermFuncDef*> * getInfoByPermIdAndPermType(list<int> * permIds,char* typeName,int *ret);
    list<PermFuncDef*> * getInfoByPermIdAndPermTypeId(list<int> * permIds,int permTypeId,int *ret);
    list<PermFuncDef*> * getInfoByPermIds(list<int> * permIds);

    void deleteInfoList(list<PermFuncDef*> * datas);
private:
    PermFuncDefData* data;
    void init();
    void initParam();
};

#endif // PERMFUNCDEFMANAGER_H
