#ifndef ALARMCONFIGPARSER_H
#define ALARMCONFIGPARSER_H

#include <QVariant>
#include "ConfHandle/configdata.h"

class AlarmConfigParser
{
public:
    AlarmConfigParser();

    static QByteArray classToJsonBytes(const QList<KeyValue*> * const config);

private:
    static QVariant* classToJsonVariant(const QList<KeyValue*> * const config);
    static QByteArray jsonVariantToJsonBytes(const QVariant* var);
};

#endif // ALARMCONFIGPARSER_H
