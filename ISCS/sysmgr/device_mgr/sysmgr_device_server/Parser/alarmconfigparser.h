#ifndef ALARMCONFIGPARSER_H
#define ALARMCONFIGPARSER_H

#include <QObject>
#include "Data/keyvalue.h"

class AlarmConfigParser : public QObject
{
    Q_OBJECT
public:
    explicit AlarmConfigParser(QObject *parent = 0);

    static QList<KeyValue*> * jsonBytesToClass(QByteArray& byteArray);
    static QVariant jsonBytesToVarivant(QByteArray& byteArray);
signals:

public slots:

private:
    static QList<KeyValue*> * jsonVariantToClass(QVariant& var);
};

#endif // ALARMCONFIGPARSER_H
