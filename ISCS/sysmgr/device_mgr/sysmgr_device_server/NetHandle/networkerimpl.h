#ifndef NETWORKERIMPL_H
#define NETWORKERIMPL_H

#include <QObject>
#ifdef Q_OS_WIN
#include <QProcess>
#endif
#include "Data/sysconfout.h"
#include "netinfomanager.h"

class NetWorkerImpl : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkerImpl(QObject *parent = nullptr);

    int collectNetInfo(QList<ConnClient*> * config,qint64 timestamp);
    void setConfig(QList<ConnClient*> * config);
    void setManager(NetInfoManager* manager);
signals:

public slots:
    void dataTimeoutSlot(qint64 timestamp);

private:
    QList<ConnClient*> * connConfigs;
    NetInfoManager* netInfoManager;
#ifdef Q_OS_WIN
    QProcess* process;
#endif

    void init();
    void initParam();
    void takeSetConfig(QList<ConnClient*> * config);
    void takeSetManager(NetInfoManager* manager);
    int takeCollectNetInfo(QList<ConnClient*> * config,qint64 timestamp);
    int isConnectedLinux(const char* ip);
    int isConnectedWin(const char* ip);
};

#endif // NETWORKERIMPL_H
