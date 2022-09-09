#include "communicatethread.h"

CommunicateThread::CommunicateThread(QObject *parent) : QThread(parent)
{

}

void CommunicateThread::run()
{
    Qt::HANDLE handle = QThread::currentThreadId();
    long id = (long)handle;
    qDebug("thread - %ld",id);

    exec();
}
