#ifndef SENDMSGWORKER_H
#define SENDMSGWORKER_H

#include <QObject>
#include "Data/sendmsg.h"

class SendMsgWorker : public QObject
{
    Q_OBJECT
public:
    explicit SendMsgWorker(QObject *parent = nullptr);

    virtual SendMsg* getData() = 0;
    virtual SendMsg* getData(qint64 timestamp) = 0;
signals:

public slots:

private:
    virtual int loadData() = 0;
    virtual int loadData(qint64 timestamp) = 0;
};

#endif // SENDMSGWORKER_H
