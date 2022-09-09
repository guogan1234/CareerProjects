#ifndef TRANSLATESERVICE_H
#define TRANSLATESERVICE_H
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>

class TransLateService
{
public:
    TransLateService();
    QString TransLateMapToJson(const QMap<int,QString> fields,const QMap<int,int> policy);
    int TransLateJsonToMap(const QString json,QMap<int,QString> &field,QMap<int,int> &policy);
    QString TransLateJsonValueToFieldName(QString value);
    int TransLateJsonValueToPolicy(QString value);
};

#endif // TRANSLATESERVICE_H
