#ifndef FULLDEVINFOPARSER_H
#define FULLDEVINFOPARSER_H

#include <QJsonDocument>
#include "Data/fulldevinfo.h"

class FullDevInfoParser
{
    public:
        FullDevInfoParser();

        static QByteArray jsonClassToByteArray(FullDevInfo* object);
        static FullDevInfo* byteArrayToJsonClass(QByteArray bArray);
        void changeByteArrayToClass(QByteArray bArray,FullDevInfo* info);
    private:
        static void jsonDocToFillClass(const QJsonDocument& doc,FullDevInfo* info);
};

#endif // FULLDEVINFOPARSER_H
