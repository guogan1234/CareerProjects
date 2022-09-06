#include "ewscomtest.h"
#include <cmath>
#include <QDebug>

EwsComtest::EwsComtest(QObject *parent):QObject(parent)
{
    mythread = new EwsThread();
    QObject::connect(mythread, &EwsThread::ewsQueryDone, this,&EwsComtest::HandlePosQueryDone);
}
EwsComtest::~EwsComtest()
{
    mythread->closeThread();
}
int EwsComtest::open_device(short iDevice)
{
    int x = s_open_device(iDevice);
    return x;
}
int EwsComtest::close_device(short iDevice)
{
    int x = s_close_device(iDevice);
    return x;
}
int EwsComtest::move_device(short iDevice, short iAxis, double fPos, double fVel, short iType)
{
    mythread->setparam(fPos, 1, iDevice, iAxis);
    mythread->startThread();
    int x = s_move_device(iDevice, iAxis, fPos, fVel, iType);
    return x;
}

int EwsComtest::stop_device(short iDevice, short iAxis)
{
    int x = s_stop_device(iDevice, iAxis);
    return x;
}
int EwsComtest::getpos(short iDevice, short iAxis, char cPos[20],double &value)
{
    double m = 0;
    int x = s_getpos(iDevice, iAxis, cPos);
    m = CharArraytodouble(cPos,20);
    value = m/1000.0;
    return x;
}
int EwsComtest::MoveByType(short iDevice, short iAxis, double fStartPos, double fEndPos, double fStepPos, double fSpeedToStart, double fSpeedToEnd, double fDeltaStep, int iTime)
{
    int x = s_MoveByType(iDevice, iAxis, fStartPos, fEndPos, fStepPos, fSpeedToStart, fSpeedToEnd, fDeltaStep, iTime);
    return x;
}
int EwsComtest::PosSignal(double fPos, ulong interval, short iDevice, short iAxis)
{
    mythread->setparam(fPos, interval,iDevice,iAxis);
    mythread->startThread();
    return 0;
}
int EwsComtest::StopQueryPos()
{
    mythread->closeThread();
    return 0;
}
void EwsComtest::HandlePosQueryDone(double pos)
{
    mythread->closeThread();
    emit posQueryDone(pos);
}
double EwsComtest::CharArraytodouble(char str[20],int size)
{
    double data = 0.0;
    int i;
    for(i=size-1;i>0;--i)
    {
        if('.'==str[i])  //'.'——46
            break;
    }
//    qDebug()<<"i="<<i;
    int counter=0;
    for(i--;i>=0;--i)
    {
        if ((str[i] >= '0') && (str[i] <= '9'))
        {

            data += int(str[i] - 0x30)*pow(10.0,counter);
//            qDebug()<<"int(str[i] - 0x30)="<<int(str[i] - 0x30)<<",data=(+)"<<data;
        }
        else if('-' == str[i])
        {

            data *= -1.0;
//            qDebug()<<"data=(-)"<<data;
        }
        counter++;
    }
    return data;
}
