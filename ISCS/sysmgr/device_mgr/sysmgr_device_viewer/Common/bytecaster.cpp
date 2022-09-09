#include "bytecaster.h"

ByteCaster::ByteCaster()
{

}

QString ByteCaster::bytesToGB(long long size)
{
    QString gb;
    double gb_d = size/(1024.0*1024.0*1024.0);
    gb.setNum(gb_d);
    gb.append(" G");
    return gb;
}

QString ByteCaster::bytesToKB(long long size)
{
    QString value;
    double temp = size/(1024.0);
    value.setNum(temp);
    value.append(" K");
    return value;
}
