#include "commanddata.h"
#include <QThread>

CommandData* CommandData::data = NULL;

CommandData *CommandData::getClass()
{
    if(NULL == data){
        data = new CommandData();
    }
    return data;
}

int CommandData::addCommandMsg(CommandMsg *msg)
{
    return takeAddCommandMsg(msg);
}

CommandMsg *CommandData::getCommandMsg()
{
    return takeGetCommandMsg();
}

CommandData::CommandData()
{
    init();
}

void CommandData::init()
{
    initParam();
}

void CommandData::initParam()
{
    commandList = new QList<CommandMsg*>();
    locker = new QReadWriteLock();
}

int CommandData::takeAddCommandMsg(CommandMsg *msg)
{
    if(!commandList || !locker) return -1;
    locker->lockForWrite();
    commandList->append(msg);
    qDebug("add:commandList len - %d",commandList->size());
    locker->unlock();
}

CommandMsg *CommandData::takeGetCommandMsg()
{
    qDebug("CommandData::takeGetCommandMsg");
    if(!commandList || !locker) return NULL;
    locker->lockForWrite();
    qDebug("get:commandList len - %d",commandList->size());
    if(commandList->isEmpty()){
        locker->unlock();
        return NULL;
    }else {
        CommandMsg* msg = commandList->takeFirst();
        locker->unlock();
        return msg;
    }
}
