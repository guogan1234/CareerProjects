#ifndef SENDMSGDATA_H
#define SENDMSGDATA_H

#include <QObject>
#include <Data/sendmsg.h>

class SendMsgData : public QObject
{
    Q_OBJECT
public:
    explicit SendMsgData(QObject *parent = nullptr);

    int loadData();
    int loadData(qint64 timestamp);
    int removeData();
    SendMsg* getData();
signals:

public slots:

private:
    SendMsg* data;

    int init();
    int initParam();
    int initData();
    int initData(qint64 timestamp);

    SendMsg* takeGetData();
};

#endif // SENDMSGDATA_H
