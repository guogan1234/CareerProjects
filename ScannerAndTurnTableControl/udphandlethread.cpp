#include "udphandlethread.h"
#include <QDebug>
#include <QMetaType>

UdpHandleThread::UdpHandleThread(WorkerQueue * workerQPtr,QString dataProtoStr,QObject *parent)
    :QThread(parent),workerQPtr_(workerQPtr),dataProtoStr_(dataProtoStr)
{
    dataProcPtr_ = new UdpDataProcessor(workerQPtr_,dataProtoStr_,this);
    qRegisterMetaType<RadarFrameBuffer>("RadarFrameBuffer");
    connect(dataProcPtr_,SIGNAL(oneFrameDataReady(RadarFrameBuffer *)), this, SLOT(HandleOneFrameData(RadarFrameBuffer *)));
}

UdpHandleThread::~UdpHandleThread()
{
    delete dataProcPtr_;
}

void UdpHandleThread::closeUdpThread()
{
    mutex_.lock();
    getOneFrameFlg_ = false;
    mutex_.unlock();
    this->wait();
}

void UdpHandleThread::GetOneRadarFrame()
{
    getOneFrameFlg_ = true;
}

void UdpHandleThread::HandleOneFrameData(RadarFrameBuffer *frameBuffer)
{
//    qDebug()<<"[UdpHandleThread]:Recv One Frame————sensor num="<<frameBuffer.sensorNum;
//    for(int i=0;i<frameBuffer.sensorNum;++i)
//    {
//        qDebug()<<"  sensorId = "<<frameBuffer.sensorBufArr[i].sensorId<<",timestamp = "<<frameBuffer.sensorBufArr[i].timestamp;
//    }
    emit oneFrameDataReady(frameBuffer);
}

void UdpHandleThread::run()
{
    while(1)
    {
        if(!getOneFrameFlg_)
        {
            if(!workerQPtr_->IsEmpty())
            {
                workerQPtr_->ClearAll();
            }
            continue;
        }
        //getOneFrameFlg == true,开始解包
        if(!workerQPtr_->IsEmpty())
        {
            workerQPtr_->ClearAll();
        }
        dataProcPtr_->GetOneRadarFrame();
        getOneFrameFlg_ = false;
    }
}
