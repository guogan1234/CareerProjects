#ifndef FULLDEVINFOPARSER_H
#define FULLDEVINFOPARSER_H

#include "Data/fulldevinfo.h"

class FullDevInfoParser
{
    public:
        FullDevInfoParser();

        static QByteArray jsonClassToByteArray(FullDevInfo* object);
};

#endif // FULLDEVINFOPARSER_H
