#include "commandhandlethread.h"

CommandHandleThread::CommandHandleThread(QObject *parent) : QThread(parent)
{

}

void CommandHandleThread::run()
{
    qDebug("CommandHandleThread::run");
    worker = new CommandHandleWorkerImpl();
    worker->setCommandManager(commandManager);
    worker->setDevInfoManager(infoManager);
    worker->handleCommand();
}

void CommandHandleThread::setCommandManager(CommandDataManager *manager)
{
    commandManager = manager;
}

void CommandHandleThread::setInfoManager(DevInfoManager *manager)
{
    infoManager = manager;
}
