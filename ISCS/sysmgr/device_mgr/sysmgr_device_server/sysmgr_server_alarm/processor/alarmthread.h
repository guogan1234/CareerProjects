#ifndef ALARMTHREAD_H
#define ALARMTHREAD_H

#include <QThread>
#include <QList>
#include <QMutex>
#include "Data/alarmmsg.h"
#include "rtdb_client.h"

class AlarmThread : public QThread
{
public:
    static AlarmThread* getInstance();

    void run();

    void setAlarmMsg(rtdbAlarmMsg* msg);
    bool isRun();
protected:
    AlarmThread(QObject* parent = NULL);

private:
    static AlarmThread* alarmThread;

    bool bRun;

    QList<rtdbAlarmMsg*> * alarmMsgList;
    QMutex* mutex;
    rtdb_cli* rtdb;

    void init();
    void initParam();

    void processAlarm();
};

#endif // ALARMTHREAD_H
