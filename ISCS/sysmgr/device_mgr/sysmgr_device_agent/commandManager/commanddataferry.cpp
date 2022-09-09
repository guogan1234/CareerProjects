#include "commanddataferry.h"

CommandDataFerry::CommandDataFerry()
{
    init();
}

int CommandDataFerry::addCommandMsg(CommandMsg *msg)
{
    return takeAddCommandMsg(msg);
}

CommandMsg *CommandDataFerry::getCommandMsg()
{
    return takeGetCommandMsg();
}

void CommandDataFerry::init()
{
    initParam();
}

void CommandDataFerry::initParam()
{
    data = CommandData::getClass();
}

int CommandDataFerry::takeAddCommandMsg(CommandMsg *msg)
{
    return data->addCommandMsg(msg);
}

CommandMsg *CommandDataFerry::takeGetCommandMsg()
{
    return data->getCommandMsg();
}
