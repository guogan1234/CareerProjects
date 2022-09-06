#ifndef EWSCOMTEST_H
#define EWSCOMTEST_H

#include <QObject>
#include "Interface.h"
#include "ewsthread.h"

class EwsComtest : public QObject
{
    Q_OBJECT
public:
    explicit EwsComtest(QObject *parent = nullptr);
    ~EwsComtest();
    int open_device(short iDevice);
    int close_device(short iDevice);
    int move_device(short iDevice, short iAxis, double fPos, double fVel, short iType);
    int stop_device(short iDevice, short iAxis);
    int getpos(short iDevice, short iAxis, char cPos[20],double &value);
    int MoveByType(short iDevice, short iAxis, double fStartPos, double fEndPos, double fStepPos, double fSpeedToStart, double fSpeedToEnd, double fDeltaStep, int iTime);
    int PosSignal(double fPos, ulong interval, short iDevice, short iAxis);
    int StopQueryPos();
private:
    double CharArraytodouble(char str[20],int size);
    EwsThread *mythread;
signals:
    void posQueryDone(double pos);
    void Positioning(double);
private slots:
    void HandlePosQueryDone(double pos);
};

#endif // EWSCOMTEST_H
