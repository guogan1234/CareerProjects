#include "nodemanager.h"

NodeManager::NodeManager(QObject *parent) : QObject(parent)
{

}

void NodeManager::setDomainNodes(QHash<int, DomainNodes *> *hash)
{
    domainNodes = hash;
}
