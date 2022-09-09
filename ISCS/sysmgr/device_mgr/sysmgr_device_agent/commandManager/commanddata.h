#ifndef COMMANDDATA_H
#define COMMANDDATA_H

#include "data/commandmsg.h"
#include <QReadWriteLock>

class CommandData
{
    public:       
        static CommandData* getClass();

        int addCommandMsg(CommandMsg* msg);
        CommandMsg* getCommandMsg();
    private:
        static CommandData* data;
        CommandData();

        QList<CommandMsg*> * commandList;
        QReadWriteLock* locker;

        void init();
        void initParam();

        int takeAddCommandMsg(CommandMsg* msg);
        CommandMsg* takeGetCommandMsg();
};

#endif // COMMANDDATA_H
