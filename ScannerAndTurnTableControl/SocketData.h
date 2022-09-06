#ifndef SOCKETDATA_H
#define SOCKETDATA_H

#include <QDateTime>

struct UdpRawData{
    char* rawData;
    unsigned long long rawDataSize;
    QDateTime timestamp;
    long long num;
};

#endif // SOCKETDATA_H
