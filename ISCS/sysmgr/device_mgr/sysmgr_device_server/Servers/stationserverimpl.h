#ifndef STATIONSERVERIMPL_H
#define STATIONSERVERIMPL_H

#include <QObject>
#include "Servers/baseserverimpl.h"
#include "Communicate/threadmanager.h"
#include "Communicate/threadmanagerferry.h"

class StationServerImpl : public BaseServerImpl
{
public:
    StationServerImpl(QList<ConnClient*> * baseConfig);

signals:
    void timeoutSignal(bool bTimeout);

private:
    bool bTimeout;
};

#endif // STATIONSERVERIMPL_H
