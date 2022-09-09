#include "alarmthread.h"
#include <string.h>
#include <QDebug>

AlarmThread* AlarmThread::alarmThread = new AlarmThread();

AlarmThread *AlarmThread::getInstance()
{
    return alarmThread;
}

void AlarmThread::run()
{
    qDebug("=== AlarmThread::run ===\n");
    processAlarm();

    exec();
}

void AlarmThread::setAlarmMsg(rtdbAlarmMsg *msg)
{ 
    mutex->lock();
    alarmMsgList->append(msg);
    int size = alarmMsgList->size();
    qDebug("#AlarmThread::setAlarmMsg - len - %d\n",size);
    mutex->unlock();
}

bool AlarmThread::isRun()
{
    return bRun;
}

AlarmThread::AlarmThread(QObject *parent):QThread(parent)
{
    init();
}

void AlarmThread::init()
{
    qDebug("=== AlarmThread::init ===\n");
    initParam();
}

void AlarmThread::initParam()
{
    bRun = false;
    alarmMsgList = new QList<rtdbAlarmMsg*>();
    mutex = new QMutex();

    int domainId = 1;
    rtdb = rtdb_cli_create(&domainId);
}

void AlarmThread::processAlarm()
{
    qDebug("=== AlarmThread::processAlarm ===\n");
    bRun = true;

    Qt::HANDLE threadId = currentThreadId();
#ifdef Q_OS_WIN
    long tId = (long)threadId;
    qDebug("#threadId - %ld\n",tId);
#endif

    while (true) {
        mutex->lock();
        int size = alarmMsgList->size();
        rtdbAlarmMsg* info = NULL;
        if(!alarmMsgList->isEmpty()){
            info = alarmMsgList->takeLast();
        }
        mutex->unlock();
//        qDebug("#list size - %d\n",size);

        if(info){
            int len = strlen(info->msg);
            qDebug("#msg len - %d\n",len);
            qDebug("#call rtdb_cli_send_message! - %s\n",info->msg);
            //调用报警推送接口
            rtdb_cli_send_message(rtdb,info->domainId,info->type,"AlarmSvr",info->msg,strlen(info->msg));
        }
        delete info;

        msleep(100);
    }
}
