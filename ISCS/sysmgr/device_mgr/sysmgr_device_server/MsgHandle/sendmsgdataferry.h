#ifndef SENDMSGDATAFERRY_H
#define SENDMSGDATAFERRY_H

#include <QObject>
#include "sendmsgdata.h"

class SendMsgDataFerry : public QObject
{
    Q_OBJECT
public:
    explicit SendMsgDataFerry(QObject *parent = nullptr);

    int loadData();
    int loadData(qint64 timestamp);
    int removeData();
    SendMsg* getData();
signals:

public slots:

private:
    SendMsgData* msgData;

    int init();
    int initParam();
};

#endif // SENDMSGDATAFERRY_H
