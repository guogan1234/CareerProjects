#ifndef WORKERQUEUE_H
#define WORKERQUEUE_H

#include <QObject>
#include <QMutex>
#include <QList>
#include "udphandledefines.h"

class WorkerQueue
{
public:
    WorkerQueue();
    ~WorkerQueue();
public:
    bool IsEmpty();
    PacketBuffer GetNextPacket();
    int GetPacketNum();
    void PushPacket(uint8_t * bufPtr,uint32_t bufSize);
    void ClearAll();
    void Free(PacketBuffer packetBuf);
private:
    QList<PacketBuffer> bufList_;
    QMutex mutex_;
};

#endif // WORKERQUEUE_H
