#ifndef FULLDEVINFO_H
#define FULLDEVINFO_H

#include "Data/devinfo.h"
#include "Data/netinfo.h"

struct FullDevInfo{
    DevInfo* devInfo;
    NetInfo* netInfo;
};
typedef struct FullDevInfo FullDevInfo;

#endif // FULLDEVINFO_H
