#ifndef RECVDATAWORKERIMPL_H
#define RECVDATAWORKERIMPL_H

#include <QObject>
#include "Data/devinfo.h"
#include "Data/fulldevinfo.h"

class RecvDataWorkerImpl : public QObject
{
        Q_OBJECT
    public:
        explicit RecvDataWorkerImpl(QObject *parent = 0);

        void setDevInfo(DevInfo* info);
        void setFullDevInfo(FullDevInfo* info);
    signals:
        void gettedDevInfoSignal();
    public slots:
        void recvDataSlot(QByteArray ba);

    private:
        DevInfo* recvInfo;
        FullDevInfo* fullRecvInfo;

        void clearDevInfoContents(DevInfo* info);
        void clearNetInfoContents(NetInfo* info);
        void clearRecvBuffer(FullDevInfo* info);
};

#endif // RECVDATAWORKERIMPL_H
