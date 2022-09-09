#ifndef DATAHANDLETHREAD_H
#define DATAHANDLETHREAD_H

#define ISCS_ALARM

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTcpSocket>
#include "Data/devinfo.h"
#include "ConfHandle/configmanager.h"
#include "ConfHandle/confhandleworker.h"

class DataHandleThread : public QThread
{
    Q_OBJECT
public:
    explicit DataHandleThread();

    void run();
    void setRawDataList(QList<QByteArray> * data,QMutex* mutex);
    void setRawDataHash(QHash<QString,QByteArray> * hash);

    void setConfigManager(ConfigManager* manager);
    void setConfHandleWorker(ConfHandleWorker* worker);
signals:

public slots:
    void dataReadySlot(QString clientIp, QByteArray ba);

private:
    QList<QByteArray> * rawDataList;
    QHash<QString,QByteArray> * rawDataHash;
    QMutex* mutex;

    QList<DevInfo*> * devInfoList;

    void init();
    void initParam();

    void takeSetRawDataList(QList<QByteArray> * data,QMutex* mutex);
    void takeSetRawDataHash(QHash<QString,QByteArray> * hash);

    ConfigManager* configManager;
    ConfHandleWorker* confWorker;
#ifdef ISCS_ALARM
    int alarmHandle(QByteArray& bArray,QString clientIp);
#endif
};

#endif // DATAHANDLETHREAD_H
