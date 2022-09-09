#ifndef CENTERSERVERIMPL_H
#define CENTERSERVERIMPL_H

#include <QObject>
#include "Servers/baseserverimpl.h"

class CenterServerImpl : public BaseServerImpl
{
public:
    CenterServerImpl(QList<ConnClient*> * baseConfig);
};

#endif // CENTERSERVERIMPL_H
