#ifndef CONFMANAGER_H
#define CONFMANAGER_H

#include <QObject>
#include "conf/confdata.h"

class ConfManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfManager(QObject *parent = 0);
    ~ConfManager();

    int initConf();
    QList<KeyValue*> * loadConf();
    int saveConf(QList<KeyValue*> * keyValues);

    const char* getValueByKey(const char* key);
    int getCollectPeriodValue(const char* key);
    int getCollectorListenPort(const char* key);
signals:

public slots:

private:
    ConfData* data;

    void init();
    void initParam();
};

#endif // CONFMANAGER_H
