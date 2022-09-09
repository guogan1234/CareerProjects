#ifndef DEVINFOPARSER_H
#define DEVINFOPARSER_H

#include "data/devinfo.h"

class DevInfoParser
{
    public:
        DevInfoParser();

        static QByteArray jsonClassToByteArray(DevInfo* info);
};

#endif // DEVINFOPARSER_H
