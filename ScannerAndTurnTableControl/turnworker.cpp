#include "turnworker.h"
#include <QDebug>

TurnWorker::TurnWorker(QObject *parent) : QObject(parent)
{
    initObject();
    angles = new TurnTableAngle();
}

TurnWorker::~TurnWorker()
{
    delete params;
    delete angles;
    ewsCom.close_device(0);
}

void TurnWorker::openDeviceSlot()
{
    qDebug("open device -- TurnWorker::openDeviceSlot");
    int ret = -1;
    ret = ewsCom.open_device(iDevice);
    if(!ret){
        qDebug()<<"open device success!";
        QString msg("open device success!");
        emit signalRefreshOpenStatus(msg);
    }else {
        qDebug()<<"open device failed!";
        QString msg("open device failed!");
        emit signalRefreshOpenStatus(msg);
    }
}

void TurnWorker::suspendDeviceSlot()
{
    ewsCom.stop_device(0,1);
    ewsCom.stop_device(0,2);
    qDebug()<<"stop the device";
}

void TurnWorker::closeDeviceSlot()
{
    ewsCom.close_device(0);
}

void TurnWorker::workProcessSlot()
{
//    for(int i = 0; xStartAngle <= 75.0; xStartAngle = xStartAngle + i*xAdd){
//        for(int j = 0; yStartAngle <= 15.0; yStartAngle = yStartAngle + j*yAdd){
//            moveToLocation(xStartAngle,yStartAngle);
//        }
//    }
    moveToLocation(xStartAngle,yStartAngle);
}

void TurnWorker::movedDoneSlot()
{
    doneCount++;

    if(doneCount%2 == 0)
    {
        movedDoneAfter();
        doneCount = 0;
    }

    if(doneCount == 1)
    {
//        yStartAngle = yStartAngle * 1000;
        iAxis = 2;
        ewsCom.move_device(iDevice,iAxis,yStartAngle * 1000,fVel* 1000,iType);
        QString msg = QString("moved to specific y angle %1").arg(yStartAngle);
        createRefreshMsg(msg);

//        //test
//        QThread::sleep(2);
////        qDebug("moved done to location - %f,%f",xAngle,yAngle);
//        movedDoneSlot();
    }

//    if(doneCount%2 == 0){
//        movedDoneAfter();
//        doneCount = 0;
//    }
}

void TurnWorker::endWriteRadarBinSlot()
{
    qDebug("end write radar bin file -- TurnWorker::endWriteRadarBinSlot");
    QString msg = "end write radar bin file!";
    createRefreshMsg(msg);

    if(xStartAngle > xMaxAngle)
    {
        return;
    }

//    yStartAngle = yStartAngle + yAdd;
//    if(yStartAngle > yMaxAngle){
//        xStartAngle = xStartAngle + xAdd;
//        yStartAngle = -15.0;
//        if(xStartAngle > xMaxAngle) return;
//    }
    switch(state)
    {
    case POSITIVE:
        yStartAngle = yStartAngle + yAdd;
        if(yStartAngle > yMaxAngle)
        {
            xStartAngle = xStartAngle + xAdd;
            //            yStartAngle = 15.0;
            yStartAngle = yMaxAngle;
            state = NEGATIVE;
            if(xStartAngle > xMaxAngle)
            {
                return;
            }
        }
        break;
    case NEGATIVE:
        yStartAngle = yStartAngle - yAdd;
        if(abs(yStartAngle) > yMaxAngle)
        {
            xStartAngle = xStartAngle + xAdd;
            //            yStartAngle = -15.0;
            yStartAngle = - yMaxAngle;
            state = POSITIVE;
            if(xStartAngle > xMaxAngle)
            {
                return;
            }
        }
        break;
    default:
        break;
    }
    moveToLocation(xStartAngle,yStartAngle);
}

void TurnWorker::recvSamplingMsg(RadarBinParams *params_)
{
    params->samples = params_->samples;
    params->chirpNum = params_->chirpNum;
    params->speed = params_->speed;
}

void TurnWorker::initObject()
{
    initParams();
    initConnects();
}

void TurnWorker::initParams()
{
    xStartAngle = -75.0;
    yStartAngle = -15.0;
//    xStartAngle =-22.0;
//    yStartAngle = -15.0;
    xAdd = 1.0;
    yAdd = 1.0;
    iDevice = 0;
    iAxis = 1;
    fVel = 10;
    iType = 0;
    xMaxAngle = 75.0;
    yMaxAngle = 15.0;

    doneCount = 0;

    /*test*/
//    xStartAngle = -75.0;
//    yStartAngle = 0.0;
//    xMaxAngle = 75.0;
//    yMaxAngle = 0.0;

    params = new RadarBinParams();

}

void TurnWorker::initConnects()
{
    connect(&ewsCom,&EwsComtest::posQueryDone,this,&TurnWorker::movedDoneSlot);
}

int TurnWorker::moveToLocation(double xAngle, double yAngle)
{
    qDebug("-----+++---------------------------------+++-----");
    qDebug("start move turntable -- TurnWorker::moveToLocation");
    int ret = -1;
    //test
//    QThread::sleep(2);
//    movedDoneSlot();
//    QThread::sleep(2);
//    qDebug("moved done to location - %f,%f",xAngle,yAngle);
//    movedDoneSlot();

//    xAngle = xAngle * 1000;
//    fVel = fVel * 1000;
    iAxis = 1;
    ret = ewsCom.move_device(iDevice,iAxis,xAngle * 1000,fVel * 1000,iType);
    QString msg = QString("moved to specific x angle %1,y angle %2").arg(xAngle).arg(yAngle);
    angles->azimuth = xAngle;
    angles->elevation = yAngle;
    emit tansAnglesMsg(angles);
    createRefreshMsg(msg);

//    movedDoneSlot();

//    yAngle = yAngle * 1000;
//    iAxis = 2;
//    ret = ewsCom.move_device(iDevice,iAxis,yAngle,fVel,iType);

    return ret;
}

void TurnWorker::getOneRadarFrame()
{
    emit getOneRadarFrameSignal();
}

void TurnWorker::sendRadarBinParams()
{
    params->samples = 512;
    params->chirpNum = 64;
    params->azimuth = 0;
    params->elevation = 0;
    params->xAngle = xStartAngle;
    params->yAngle = yStartAngle;
    emit writeRadarBinParams(params);
}

void TurnWorker::movedDoneAfter()
{
    qDebug("start get one radar frame -- TurnWorker::movedDoneAfter");
    QString msg = "start get one radar frame";
    createRefreshMsg(msg);

    sendRadarBinParams();
    getOneRadarFrame();
}

void TurnWorker::createRefreshMsg(QString &msg)
{
    qDebug()<<"TurnWorker::createRefreshMsg"<<msg;
    emit signalRefreshStatus(msg);
}
