#include "alarmconfigparser.h"
#include <QJsonDocument>
#include <QVariant>

AlarmConfigParser::AlarmConfigParser(QObject *parent) : QObject(parent)
{

}

QList<KeyValue *> *AlarmConfigParser::jsonBytesToClass(QByteArray &byteArray)
{
    QVariant var;
    var = jsonBytesToVarivant(byteArray);
    return jsonVariantToClass(var);
}

QVariant AlarmConfigParser::jsonBytesToVarivant(QByteArray &byteArray)
{
    QJsonDocument doc = QJsonDocument::fromJson(byteArray);
    return doc.toVariant();
}

QList<KeyValue *> *AlarmConfigParser::jsonVariantToClass(QVariant &var)
{
    if(var.isNull()) return NULL;
    QList<KeyValue*> * retList = NULL;
    if(var.canConvert<QVariantList>()){
        QVariantList list;
        list = var.toList();

        retList = new QList<KeyValue*>();
        for(int i = 0;i < list.size();i++){
            QVariant varObj = list.at(i);
            if(varObj.canConvert<QVariantMap>()){
                KeyValue* temp = new KeyValue();
                QVariantMap varMap = varObj.toMap();
                QVariant varKey = varMap.value("key");
                QVariant varValue = varMap.value("value");
                if(varKey.canConvert<QByteArray>()){
                    QByteArray ba = varKey.toByteArray();
                    temp->key = strdup(ba.data());
                }
                if(varValue.canConvert<QByteArray>()){
                    QByteArray ba = varValue.toByteArray();
                    temp->value = strdup(ba.data());
                }
                retList->append(temp);
            }
        }
    }
    return retList;
}
