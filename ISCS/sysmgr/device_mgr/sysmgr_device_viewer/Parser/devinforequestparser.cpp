#include "devinforequestparser.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>

DevInfoRequestParser::DevInfoRequestParser()
{

}

QByteArray DevInfoRequestParser::jsonClassToByteArray(DevInfoRequest *dev)
{
    QByteArray bArray;
    if(!dev) return bArray;

    QList<QString> * ips = dev->ips;
    QJsonArray ipArray;
    if(ips){
        for(int i = 0;i < ips->size(); ++i){
            QJsonValue value(ips->at(i));
            ipArray.append(value);
        }
    }
    QJsonObject root;
    root.insert("name",QJsonValue(dev->name));
    root.insert("ips",QJsonValue(ipArray));
    QJsonDocument doc;
    doc.setObject(root);
    return doc.toJson(QJsonDocument::Compact);
}
