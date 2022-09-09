#ifndef DEVINFOREQUESTPARSER_H
#define DEVINFOREQUESTPARSER_H

#include <QByteArray>
#include "Data/devinforequest.h"

class DevInfoRequestParser
{
    public:
        DevInfoRequestParser();

        static QByteArray jsonClassToByteArray(DevInfoRequest* dev);
};

#endif // DEVINFOREQUESTPARSER_H
