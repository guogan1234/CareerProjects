#ifndef COMMANDDATAFERRY_H
#define COMMANDDATAFERRY_H

#include "commanddata.h"

class CommandDataFerry
{
    public:
        CommandDataFerry();

        int addCommandMsg(CommandMsg* msg);
        CommandMsg* getCommandMsg();
    private:
        CommandData* data;
        void init();
        void initParam();

        int takeAddCommandMsg(CommandMsg *msg);
        CommandMsg* takeGetCommandMsg();
};

#endif // COMMANDDATAFERRY_H
