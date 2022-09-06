#include "workerqueue.h"
#include <QMutex>
#include <QDebug>
#include "SocketData.h"

extern QMutex udpMutex;
extern QList<UdpRawData*> rawDataList;

WorkerQueue::WorkerQueue()
{

}

WorkerQueue::~WorkerQueue()
{

}

//char *WorkerQueue::getNextPacket()
//{
//    if(!isEmpty()){
//        udpMutex.lock();
//        qDebug()<<"list length(before take):"<<rawDataList.length();
//        UdpRawData* udpRawData = nullptr;
//        udpRawData = rawDataList.takeFirst();
//        qDebug()<<"list length(after take):"<<rawDataList.length();
//        udpMutex.unlock();

//        if(udpRawData){
//            qDebug()<<"msg:"<<udpRawData->rawData;
//            return udpRawData->rawData;
//        }
//    }
//    return nullptr;
//}

bool WorkerQueue::IsEmpty()
{
    return rawDataList.isEmpty();
}

PacketBuffer WorkerQueue::GetNextPacket()
{
    PacketBuffer ret;
    if(!IsEmpty()){
        udpMutex.lock();
//        qDebug()<<"list length(before take):"<<rawDataList.length();
        UdpRawData* udpRawData = nullptr;
        udpRawData = rawDataList.takeFirst();
//        qDebug()<<"list length(after take):"<<rawDataList.length();
//        udpMutex.unlock();

        if(udpRawData){
//            qDebug()<<"msg:"<<udpRawData->rawData;
            ret.bufPtr = (uint8_t*)udpRawData->rawData;
            ret.bufSize = udpRawData->rawDataSize;
        }
        udpMutex.unlock();
    }
    return ret;
}

int WorkerQueue::GetPacketNum()
{
    return rawDataList.size();
}

void WorkerQueue::PushPacket(uint8_t *bufPtr, uint32_t bufSize)
{
    qDebug()<<bufSize<<bufPtr;
}

void WorkerQueue::ClearAll()
{
    udpMutex.lock();
    foreach (UdpRawData* temp, rawDataList) {
        delete[] temp->rawData;
    }
    rawDataList.clear();
    udpMutex.unlock();
}

void WorkerQueue::Free(PacketBuffer packetBuf)
{
    delete[] packetBuf.bufPtr;
}
