#include "ewsthread.h"
#include <QDebug>
#include <cmath>

EwsThread::EwsThread(QObject *parent) : QThread(parent)
{
    pos_ = 0.0;
    interUs_ = 0;
    iDevice_ = 0;
    iAxis_ = 1;
    timeoutCounts_ = 4;
}

void EwsThread::closeThread()
{
    mutex_.lock();
    runningFlg_ = false;
    mutex_.unlock();
    this->wait();
}

void EwsThread::startThread()
{
    if(!runningFlg_)
    {
        runningFlg_ = true;
        this->start();
    }
}

void EwsThread::run()
{
    char cPos[20];
    double curr_pos = 0.;
    double last_pos = 0.;
    bool isFirstFlg = true;
    bool posDoneFlg = false;
    int timeoutCounter=0;
    mutex_.lock();
    while (runningFlg_)
    {
        int ret = s_getpos(iDevice_, iAxis_, cPos);
        if(ret!=0)//通信失败
        {
            qDebug()<<"[ERROR]:s_getpos failed!";
            continue;
        }
        curr_pos = CharArraytodouble(cPos);
        if(IsPosEqual(curr_pos,pos_))
        {
            posDoneFlg = true;
            break;
        }
        if(isFirstFlg)
        {
            isFirstFlg = false;
            last_pos = curr_pos;
        }
        if(abs(last_pos-curr_pos)<0.001)    //当前位置未更新
        {
            timeoutCounter++;
        }
        if(timeoutCounter == timeoutCounts_)
        {
            qDebug()<<"[ERROR]:EWS query timeout!";
            posDoneFlg = false;
            break;
        }
        last_pos = curr_pos;
        sleep(interUs_);
    }
    if(posDoneFlg)
    {
        emit ewsQueryDone(curr_pos/1000.0);
    }
    else
    {
        emit ewsQueryDone(65535.0);
    }
    mutex_.unlock();

}
void EwsThread::setparam(double fPos, ulong interUs, short iDevice, short iAxis)
{
    pos_ = fPos;
    interUs_ = interUs;
    iDevice_ = iDevice;
    iAxis_ = iAxis;
}
double EwsThread::CharArraytodouble(char str[20])
{
    double data = 0.0;
    int i;
    int size = 20;
    for(i=size-1;i>0;--i)
    {
        if('.'==str[i])  //'.'——46
            break;
    }
    qDebug()<<"i="<<i;
    int counter=0;
    for(i--;i>=0;--i)
    {
        if ((str[i] >= '0') && (str[i] <= '9'))
        {

            data += int(str[i] - 0x30)*pow(10.0,counter);
            qDebug()<<"int(str[i] - 0x30)="<<int(str[i] - 0x30)<<",data=(+)"<<data;
        }
        else if('-' == str[i])
        {

            data *= -1.0;
            qDebug()<<"data=(-)"<<data;
        }
        counter++;
    }
    return data;
}

bool EwsThread::IsPosEqual(double pos1,double pos2)
{
    qDebug()<<"[EWS]:current pos ="<<pos1<<", dest post ="<<pos2;
    if(abs(pos1-pos2)<=2.0) //0.002 rad
    {
        return true;
    }
    return false;
}
