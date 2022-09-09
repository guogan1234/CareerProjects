#ifndef ALARMMSGPARSER_H
#define ALARMMSGPARSER_H

#include "Data/alarmmsg.h"
#include <QByteArray>

class AlarmMsgParser
{
public:
    AlarmMsgParser();

    static char* classToJsonString(AlarmMsg* msg);
    static QByteArray classToJsonByteArray(AlarmMsg *msg);
};

#endif // ALARMMSGPARSER_H
