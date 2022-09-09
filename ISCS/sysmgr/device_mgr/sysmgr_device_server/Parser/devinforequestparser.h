#ifndef DEVINFOREQUESTPARSER_H
#define DEVINFOREQUESTPARSER_H

#include <QByteArray>
#include "Data/devinforequest.h"

class DevInfoRequestParser
{
    public:
        DevInfoRequestParser();

        static QByteArray jsonClassToByteArray(DevInfoRequest* dev);
        static DevInfoRequest* byteArrayToJsonClass(QByteArray bArray);
};

#endif // DEVINFOREQUESTPARSER_H
