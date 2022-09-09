#ifndef COMMANDDATAMANAGER_H
#define COMMANDDATAMANAGER_H

#include "commanddataferry.h"

class CommandDataManager
{
    public:
        CommandDataManager();

        int addCommandMsg(CommandMsg* msg);
        CommandMsg* getCommandMsg();
    private:
        CommandDataFerry* ferry;
        void init();
        void initParam();

        int takeAddCommandMsg(CommandMsg* msg);
        CommandMsg* takeGetCommandMsg();
};

#endif // COMMANDDATAMANAGER_H
