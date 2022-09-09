#include "commandmanager.h"

CommandManager::CommandManager(QObject *parent) : QObject(parent)
{
    init();
}

QByteArray CommandManager::getCommand()
{
    lock->lockForRead();
    if(commandList->isEmpty()){
        lock->unlock();
        return QByteArray();
    }else{
        //improve
        QByteArray array = commandList->takeFirst();
        lock->unlock();
        return array;
    }
}

int CommandManager::addCommand(QByteArray bArray)
{
    lock->lockForWrite();
    commandList->append(bArray);
    lock->unlock();

    return 0;
}

void CommandManager::init()
{
    initParam();
}

void CommandManager::initParam()
{
    commandList = new QList<QByteArray>();
    lock = new QReadWriteLock();
}
