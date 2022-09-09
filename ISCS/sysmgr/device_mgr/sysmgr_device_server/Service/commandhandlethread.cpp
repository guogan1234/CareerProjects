#include "commandhandlethread.h"

CommandHandleThread::CommandHandleThread(QObject *parent) : QThread(parent)
{
    init();
}

void CommandHandleThread::setRawInfoList(QList<QByteArray> *list, QMutex *mutex)
{
    finder->setRawInfoList(list,mutex);
}

void CommandHandleThread::setRawInfoHash(QHash<QString, QByteArray> *hash)
{
    finder->setRawInfoHash(hash);
}

void CommandHandleThread::setNetInfoMap(QMap<QString, NetInfo *> *map, QReadWriteLock *lock)
{
    finder->setNetInfoMap(map,lock);
}

void CommandHandleThread::run()
{
    bRun = true;
//    while (bRun) {
//    }
    worker = new CommandHandleWorkerImpl();
    worker->setDataFinder(finder);
    connect(this,SIGNAL(recvCommandSignal(QTcpSocket*,QByteArray)),worker,SLOT(recvMessageSlot(QTcpSocket*,QByteArray)));
    connect(worker,SIGNAL(sendResponseSignal(QTcpSocket*,QByteArray)),this,SIGNAL(sendResponseSignal(QTcpSocket*,QByteArray)));

    exec();
}

void CommandHandleThread::init()
{
    initParam();
}

void CommandHandleThread::initParam()
{
    finder = new DataFinder();
    commandManager = new CommandManager();
    bRun = false;
}
