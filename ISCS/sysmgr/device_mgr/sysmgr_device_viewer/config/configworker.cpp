#include "configworker.h"
#include "Parser/alarmconfigparser.h"
#include <QTcpSocket>

#define CONFIG_PORT 9102

ConfigWorker::ConfigWorker(QObject *parent) : QObject(parent)
{

}

void ConfigWorker::setConfig(QList<KeyValue *> *configData)
{
    config = configData;
}

void ConfigWorker::setConnectInfo(QList<ConnClient *> *info)
{
    connInfo = info;
}

void ConfigWorker::handOutConfigSlot()
{
    QByteArray ba;
    ba = AlarmConfigParser::classToJsonBytes(config);
    qDebug()<<"[alarm bytes]"<<ba;

    QTcpSocket* sock = new QTcpSocket();
    ConnClient* temp = connInfo->at(0);
    QList<QString> * connIps = NULL;
    if(temp){
        connIps = temp->ips;
    }
    if(connIps){
        for(int i = 0;i < connIps->size();i++){
            sock->connectToHost(connIps->at(i),CONFIG_PORT);
            if(sock->waitForConnected()){
                ba.append('\n');
                sock->write(ba);
                sock->waitForBytesWritten();
                break;
            }
        }
    }
}
