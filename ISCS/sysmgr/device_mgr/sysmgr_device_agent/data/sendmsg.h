#ifndef SENDMSG_H
#define SENDMSG_H

#include <QString>

class SendMsg
{
public:
    SendMsg() {}

    void setSeq(qint64 seq)
    {
        this->seq = seq;
    }

    void setCommand(QString command)
    {
        this->command = command;
    }

    qint64 getSeq()
    {
        return seq;
    }

    QString getCommand()
    {
        return command;
    }
private:
    qint64 seq;
    QString command;
};

#endif // SENDMSG_H

