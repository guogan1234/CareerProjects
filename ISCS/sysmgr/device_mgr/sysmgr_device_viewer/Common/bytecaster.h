#ifndef BYTECASTER_H
#define BYTECASTER_H

#include <QString>

class ByteCaster
{
    public:
        ByteCaster();

        static QString bytesToGB(long long size);
        static QString bytesToKB(long long size);
};

#endif // BYTECASTER_H
