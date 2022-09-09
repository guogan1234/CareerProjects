#include "uimodel.h"
#include "confcmn.h"

UIModel::UIModel(QObject *parent) : QObject(parent)
{

}

int UIModel::startCFDBService()
{
    return confdb_init();
}

int UIModel::endCFDBService()
{
    return confdb_release();
}
