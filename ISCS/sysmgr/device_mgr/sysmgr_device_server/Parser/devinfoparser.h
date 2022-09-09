#ifndef DEVINFOPARSER_H
#define DEVINFOPARSER_H

#include "Data/devinfo.h"
#include <QJsonDocument>

class DevInfoParser
{
public:
    DevInfoParser();

    static QByteArray jsonClassToByteArray(DevInfo* info);
    static DevInfo* jsonToClass(const QByteArray& json);
    void changeByteArrayToClass(const QByteArray& json,DevInfo* info);

private:
    static void jsonDocToFillClass(const QJsonDocument& doc, DevInfo* dev);
};

#endif // DEVINFOPARSER_H
