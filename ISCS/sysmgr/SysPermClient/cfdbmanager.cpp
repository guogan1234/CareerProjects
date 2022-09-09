#include "cfdbmanager.h"
#include "confcmn.h"

CFDBManager::CFDBManager(QObject *parent) : QObject(parent)
{

}

int CFDBManager::startCFDBModule()
{
    return confdb_init();
}

int CFDBManager::endCFDBModule()
{
    return confdb_release();
}
