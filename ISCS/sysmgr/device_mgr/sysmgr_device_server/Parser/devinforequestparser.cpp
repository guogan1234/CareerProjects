#include "devinforequestparser.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QVariant>
#include <QDebug>

DevInfoRequestParser::DevInfoRequestParser()
{

}

QByteArray DevInfoRequestParser::jsonClassToByteArray(DevInfoRequest *dev)
{
    QByteArray bArray;
    if(!dev) return bArray;

    QList<QString> * ips = dev->ips;
    QJsonArray ipArray;
    for(int i = 0;i < ips->size(); ++i){
        QJsonValue value(ips->at(i));
        ipArray.append(value);
    }
    QJsonObject root;
    root.insert("name",QJsonValue(dev->name));
    root.insert("ips",QJsonValue(ipArray));
    QJsonDocument doc;
    doc.setObject(root);
    return doc.toJson(QJsonDocument::Compact);
}

DevInfoRequest *DevInfoRequestParser::byteArrayToJsonClass(QByteArray bArray)
{
    QJsonParseError jsonError;
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(bArray,&jsonError);
    if(jsonError.error!=QJsonParseError::NoError || doc.isNull()){
        qDebug()<<"[Json Parse Error]"<<jsonError.errorString();
        return NULL;
    }

    DevInfoRequest* request = NULL;
    qDebug()<<"isObject - "<<doc.isObject();
    QVariant variant = doc.toVariant();
    if(variant.canConvert<QVariantMap>()){
        request = new DevInfoRequest();
        QVariantMap variantMap = variant.toMap();
        QVariant varName = variantMap.value("name");
        if(varName.canConvert<QString>()){
            QString name = varName.toString();
            request->name = name;
        }
        QVariant varIps = variantMap.value("ips");
        if(varIps.canConvert<QVariantList>()){
            QList<QString> * ipList = new QList<QString>();
            QVariantList varList = varIps.toList();
            for(int i = 0;i < varList.size(); ++i){
                QVariant var = varList.at(i);
                if(var.canConvert<QString>()){
                    QString ip = var.toString();
                    ipList->append(ip);
                }
            }
            request->ips = ipList;
        }
    }
    return request;
}
