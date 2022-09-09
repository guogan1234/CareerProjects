#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <QList>
#include "Data/keyvalue.h"

#define CONF_FILE "conf/iscs_sysDevServer.conf"
#define CONF_FILE_OUT "conf/iscs_sysDevServer_temp.conf"

class ConfigData
{
public:
    ConfigData();
    ~ConfigData();

    int initConf();
    QList<KeyValue*> * loadConf();
    int saveConf(QList<KeyValue*> * keyValues);

    const char* getValueByKey(const char* key);
private:
    QList<KeyValue*> * confCache;
};

#endif // CONFIGDATA_H
