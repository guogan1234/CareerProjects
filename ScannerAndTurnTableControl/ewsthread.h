#ifndef EWSTHREAD_H
#define EWSTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "Interface.h"

class EwsThread : public QThread
{
    Q_OBJECT
public:
    explicit EwsThread(QObject *parent = nullptr);
    void closeThread();
    void startThread();
    void setparam(double fPos, ulong interUs, short iDevice, short iAxis);
protected:
    virtual void run();
private:
    double CharArraytodouble(char str[20]);
    bool IsPosEqual(double pos1,double pos2);
public slots:

signals:
    void ewsQueryDone(double);
private:
    double pos_;
    ulong interUs_;
    int timeoutCounts_;
    short iDevice_;
    short iAxis_;

    bool runningFlg_ = false;
    QMutex mutex_;
};

#endif // EWSTHREAD_H
