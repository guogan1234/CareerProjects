#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include "configdata.h"
#include "Data/alarmconfig.h"

class ConfigManager:public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = 0);
    ~ConfigManager();

    int initConf();
    QList<KeyValue*> * loadConf();
    int saveConf(QList<KeyValue*> * keyValues);

    const char* getValueByKey(const char* key);
    int getValueByKeyToInt(const char* key);
    int getCollectPeriodValue(const char* key);
    int getConnectPort(const char* key);
    int getServicePort(const char* key);
    int getSustainTime(const char* key);
    int getCpuLimit(const char* key);
    int getMemLimit(const char* key);
    int getDiskLimit(const char* key);
    AlarmConfig* getAlarmConfig();
signals:

public slots:

private:
    ConfigData* data;

    void init();
    void initParam();
};

#endif // CONFIGMANAGER_H
