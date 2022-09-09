#ifndef CONFDATA_H
#define CONFDATA_H

#include <QList>

#define CONF_FILE "conf/iscs_sysDevCollector.conf"
#define CONF_FILE_OUT "conf/iscs_sysDevCollector_temp.conf"

struct KeyValue{
    char* key;
    char* value;
};
typedef struct KeyValue KeyValue;

class ConfData
{
public:
    ConfData();
    ~ConfData();

    int initConf();
    QList<KeyValue*> * loadConf();
    int saveConf(QList<KeyValue*> * keyValues);

    const char* getValueByKey(const char* key);
private:
    QList<KeyValue*> * confCache;
};

#endif // CONFDATA_H
