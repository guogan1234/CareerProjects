#ifndef CONFIGWORKERIMPL_H
#define CONFIGWORKERIMPL_H

#include <QObject>

class ConfigWorkerImpl : public QObject
{
    Q_OBJECT
public:
    explicit ConfigWorkerImpl(QObject *parent = 0);

signals:

public slots:
    void dataIncome(QByteArray datas);
};

#endif // CONFIGWORKERIMPL_H
