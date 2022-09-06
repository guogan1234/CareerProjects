#ifndef UDPHANDLETHREAD_H
#define UDPHANDLETHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "udpdataprocessor.h"

class UdpHandleThread : public QThread
{
    Q_OBJECT
public:
    //[dataProtoStr Options]:CaptureOnly/AntData/
    explicit UdpHandleThread(WorkerQueue * workerQPtr,QString dataProtoStr="CaptureOnly",QObject *parent = nullptr);
    ~UdpHandleThread();
    void closeUdpThread();
    void GetOneRadarFrame();
public slots:
    void HandleOneFrameData(RadarFrameBuffer *frameBuffer);
protected:
    void run();

signals:
    void oneFrameDataReady(RadarFrameBuffer *frameBuffer);
private:
    UdpDataProcessor * dataProcPtr_;
    WorkerQueue * workerQPtr_;

    bool getOneFrameFlg_ = false;

    QString dataProtoStr_ = "CaptureOnly";//[Options]:CaptureOnly/AntData/

    QMutex mutex_;
};

#endif // UDPHANDLETHREAD_H
