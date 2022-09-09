#include "permfuncdefmanager.h"

PermFuncDefManager::PermFuncDefManager()
{
    init();
}

PermFuncDefManager::~PermFuncDefManager()
{
    delete data;
}

list<PermFuncDef *> *PermFuncDefManager::getInfoByPermIdAndPermType(list<int> *permIds, char *typeName, int *ret)
{
    return data->getInfoByPermIdAndPermType(permIds,typeName,ret);
}

list<PermFuncDef *> *PermFuncDefManager::getInfoByPermIdAndPermTypeId(list<int> *permIds, int permTypeId, int *ret)
{
    return data->getInfoByPermIdAndPermTypeId(permIds,permTypeId,ret);
}

list<PermFuncDef *> *PermFuncDefManager::getInfoByPermIds(list<int> *permIds)
{
    return data->getInfoByPermIds(permIds);
}

void PermFuncDefManager::deleteInfoList(list<PermFuncDef *> *datas)
{
    return data->deleteInfoList(datas);
}

void PermFuncDefManager::init()
{
    initParam();
}

void PermFuncDefManager::initParam()
{
    data = new PermFuncDefData();
}
