#include "alarmconfigparser.h"
#include <QJsonDocument>

AlarmConfigParser::AlarmConfigParser()
{

}

QByteArray AlarmConfigParser::classToJsonBytes(const QList<KeyValue *> * const config)
{
    QVariant* temp = classToJsonVariant(config);
    return jsonVariantToJsonBytes(temp);
}

QVariant *AlarmConfigParser::classToJsonVariant(const QList<KeyValue *> * const config)
{
    if(!config) return NULL;
    QVariantList list;
    foreach (KeyValue* temp, *config) {
        if(temp){
            QVariantMap varMap;
            varMap.insert("key",QVariant(temp->key));
            varMap.insert("value",QVariant(temp->value));

            list.append(QVariant(varMap));
        }
    }
    QVariant* ret = new QVariant(list);
    return ret;
}

QByteArray AlarmConfigParser::jsonVariantToJsonBytes(const QVariant *var)
{
    QByteArray ret;
    QJsonDocument doc = QJsonDocument::fromVariant(*var);
    if(doc.isNull()) return ret;
    return doc.toJson(QJsonDocument::Compact);
}
