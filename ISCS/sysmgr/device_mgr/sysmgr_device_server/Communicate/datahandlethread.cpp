#include "datahandlethread.h"
#include <QHostAddress>
#include "Parser/devinfoparser.h"
#include <QDebug>

#include "sysmgr_server_alarm.h"

DataHandleThread::DataHandleThread()
{
    init();
}

void DataHandleThread::run()
{

}

void DataHandleThread::setRawDataList(QList<QByteArray> *data, QMutex *mutex)
{
    takeSetRawDataList(data,mutex);
}

void DataHandleThread::setRawDataHash(QHash<QString, QByteArray> *hash)
{
    takeSetRawDataHash(hash);
}

void DataHandleThread::setConfigManager(ConfigManager *manager)
{
    configManager = manager;
}

void DataHandleThread::setConfHandleWorker(ConfHandleWorker *worker)
{
    confWorker = worker;
}

void DataHandleThread::dataReadySlot(QString clientIp, QByteArray ba)
{
    int recvLen = ba.length();
    qDebug("recvLen - %d",recvLen);
    mutex->lock();
    rawDataList->append(ba);
    rawDataHash->insert(clientIp,ba);
    mutex->unlock();

    alarmHandle(ba,clientIp);
}

void DataHandleThread::init()
{
    initParam();
}

void DataHandleThread::initParam()
{

}

void DataHandleThread::takeSetRawDataList(QList<QByteArray> *data, QMutex *mutex)
{
    this->rawDataList = data;
    this->mutex = mutex;
}

void DataHandleThread::takeSetRawDataHash(QHash<QString, QByteArray> *hash)
{
    rawDataHash = hash;
}

int DataHandleThread::alarmHandle(QByteArray &bArray, QString clientIp)
{
#ifdef ISCS_ALARM
    DevInfo* devInfo = DevInfoParser::jsonToClass(bArray);
    AlarmConfig* alarmConfig = configManager->getAlarmConfig();
    AlarmExtra* alarmExtra = confWorker->getAlarmExtraByIP(clientIp);
//    Sysmgr_server_alarm alarmHandler;
//    alarmHandler.alarmHandle(alarmConfig,alarmExtra,devInfo);
#endif
    return 0;
}
