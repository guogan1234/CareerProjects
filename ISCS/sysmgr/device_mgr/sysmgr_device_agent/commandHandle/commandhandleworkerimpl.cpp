#include "commandhandleworkerimpl.h"
#include "parser/commandmsgparser.h"
#include "parser/devinfoparser.h"
#include <QThread>

#include "conf/confmanager.h"

CommandHandleWorkerImpl::CommandHandleWorkerImpl(QObject *parent) : CommandHandleWorker(parent)
{

}

void CommandHandleWorkerImpl::setCommandManager(CommandDataManager *manager)
{
    takeSetCommandManager(manager);
}

void CommandHandleWorkerImpl::setDevInfoManager(DevInfoManager *manager)
{
    takeSetDevInfoManager(manager);
}

int CommandHandleWorkerImpl::handleCommand()
{
    qDebug("CommandHandleWorkerImpl::handleCommand");
    while (true) {
        CommandMsg* msg = commandManager->getCommandMsg();
        qDebug("get a command - %p",msg);
        QTcpSocket* socket = NULL;
        if(msg){
            SendMsg* sendCommand = NULL;
            sendCommand = CommandMsgParser::jsonCommandToClass(msg->command);

            socket = msg->clientSocket;
            QString command = sendCommand->getCommand();
            if(command == "data"){
                handleDataCommand(socket,sendCommand);
            }
            else if(command == "conf"){
                handleConfCommand(sendCommand);
            }

//            DevInfo* info = infoManager->getDevInfo();
//            if(info && sendCommand){
//                info->seq = sendCommand->getSeq();
//            }
//            QByteArray infoMsg = DevInfoParser::jsonClassToByteArray(info);
//            infoMsg.append('\n');
//            qint64 infoLen = infoMsg.length();

//            if(socket){
//                qDebug("socket send data - %d",infoMsg.length());
////                socket->write("this is data");
//                qint64 writeLen = socket->write(infoMsg);
//                qDebug("msgLen - %lld ,writeLen - %lld",infoLen,writeLen);
//                socket->flush();
//            }
        }
        QThread::msleep(1000);
    }

    return 0;
}

void CommandHandleWorkerImpl::takeSetCommandManager(CommandDataManager *manager)
{
    commandManager = manager;
}

void CommandHandleWorkerImpl::takeSetDevInfoManager(DevInfoManager *manager)
{
    infoManager = manager;
}

int CommandHandleWorkerImpl::handleDataCommand(QTcpSocket *socket, SendMsg *sendCommand)
{
    qDebug("CommandHandleWorkerImpl::handleDataCommand\n");
    DevInfo* info = infoManager->getDevInfo();
    if(info && sendCommand){
        info->seq = sendCommand->getSeq();
    }
    QByteArray infoMsg = DevInfoParser::jsonClassToByteArray(info);
    infoMsg.append('\n');
    qint64 infoLen = infoMsg.length();

    if(socket){
        qDebug("socket send data - %d",infoMsg.length());
//                socket->write("this is data");
        qint64 writeLen = socket->write(infoMsg);
        qDebug("msgLen - %lld ,writeLen - %lld",infoLen,writeLen);
        socket->flush();
    }
}

int CommandHandleWorkerImpl::handleConfCommand(SendMsg *sendCommand)
{
    qDebug("CommandHandleWorkerImpl::handleConfCommand\n");
    qint64 seq;
    seq = sendCommand->getSeq();
    QString command;
    command = sendCommand->getCommand();
    QList<KeyValue*> * keyValues = new QList<KeyValue*>();
    ConfManager* confManager = new ConfManager();
    confManager->saveConf(keyValues);

    return 0;
}
