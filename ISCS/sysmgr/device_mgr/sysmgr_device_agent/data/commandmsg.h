#ifndef COMMANDMSG_H
#define COMMANDMSG_H

#include <QTcpSocket>

struct CommandMsg{
        QTcpSocket* clientSocket;
        QByteArray command;
};
typedef struct CommandMsg CommandMsg;

#endif // COMMANDMSG_H
