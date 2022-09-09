#include "commanddatamanager.h"

CommandDataManager::CommandDataManager()
{
    init();
}

int CommandDataManager::addCommandMsg(CommandMsg *msg)
{
    return takeAddCommandMsg(msg);
}

CommandMsg *CommandDataManager::getCommandMsg()
{
    return takeGetCommandMsg();
}

void CommandDataManager::init()
{
    initParam();
}

void CommandDataManager::initParam()
{
    ferry = new CommandDataFerry();
}

int CommandDataManager::takeAddCommandMsg(CommandMsg *msg)
{
    return ferry->addCommandMsg(msg);
}

CommandMsg *CommandDataManager::takeGetCommandMsg()
{
    qDebug("CommandDataManager::takeGetCommandMsg");
    return ferry->getCommandMsg();
}
