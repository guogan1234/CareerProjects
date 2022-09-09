#include "recvdataworkerimpl.h"
#include "Parser/devinfoparser.h"
#include "Parser/fulldevinfoparser.h"
#include <QDateTime>
#include <QDebug>

RecvDataWorkerImpl::RecvDataWorkerImpl(QObject *parent) : QObject(parent)
{

}

void RecvDataWorkerImpl::setDevInfo(DevInfo *info)
{
    recvInfo = info;
}

void RecvDataWorkerImpl::setFullDevInfo(FullDevInfo *info)
{
    fullRecvInfo = info;
}

int receiveCount = 0;
void RecvDataWorkerImpl::recvDataSlot(QByteArray ba)
{
    qDebug("RecvDataWorkerImpl::recvDataSlot - %d",receiveCount);
    qDebug()<<"[realStartTime]"<<QDateTime::currentDateTime()<<receiveCount;
    receiveCount++;
    //清空上一次数据，防止内存泄露
    clearRecvBuffer(fullRecvInfo);
    FullDevInfoParser parser;
    parser.changeByteArrayToClass(ba,fullRecvInfo);

    emit gettedDevInfoSignal();
}

void RecvDataWorkerImpl::clearDevInfoContents(DevInfo *info)
{
//    qDebug("RecvDataWorkerImpl::clearDevInfoContents...");
    if(!info) return;
    QList<Process*> * processList = NULL;
    processList = info->procList;
    if(processList){
        while (!processList->isEmpty()) {
            delete processList->takeFirst();
        }
    }
    QList<Cpu*> * cpuList = NULL;
    cpuList = info->cpuList;
    if(cpuList){
        while (!cpuList->isEmpty()) {
            delete cpuList->takeFirst();
        }
    }
    QList<Disk*> * diskList = NULL;
    diskList = info->diskList;
    if(diskList){
        while (!diskList->isEmpty()) {
            delete diskList->takeFirst();
        }
    }
    Memory* memory = NULL;
    memory = info->memory;
    if(memory){
        delete memory;
    }
    delete info;
    qDebug("normal exit RecvDataWorkerImpl::clearDevInfoContents");
}

void RecvDataWorkerImpl::clearNetInfoContents(NetInfo *info)
{
    if(!info) return;
    delete info;
}

void RecvDataWorkerImpl::clearRecvBuffer(FullDevInfo *info)
{
    if(!info) return;
    DevInfo* devInfo = info->devInfo;
    NetInfo* netInfo = info->netInfo;
    clearDevInfoContents(devInfo);
    clearNetInfoContents(netInfo);
    info->devInfo = NULL;
    info->netInfo = NULL;
}
