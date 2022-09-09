#include "cfdbmanager.h"
#include "confcmn.h"

CFDBManager::CFDBManager()
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
