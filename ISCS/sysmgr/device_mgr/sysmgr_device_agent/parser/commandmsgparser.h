#ifndef COMMANDMSGPARSER_H
#define COMMANDMSGPARSER_H

#include "data/sendmsg.h"

class CommandMsgParser
{
    public:
        CommandMsgParser();

        static SendMsg* jsonCommandToClass(QByteArray& bArray);
};

#endif // COMMANDMSGPARSER_H
