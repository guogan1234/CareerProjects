#include "commandmsgparser.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QVariant>
#include <QDebug>

CommandMsgParser::CommandMsgParser()
{

}

SendMsg *CommandMsgParser::jsonCommandToClass(QByteArray &bArray)
{
    SendMsg* ret = NULL;

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(bArray,&jsonError);
    if(!doc.isNull() && jsonError.error == QJsonParseError::NoError){
        QVariant var = doc.toVariant();
        if(!var.isNull()){
            ret = new SendMsg();
            if(var.canConvert<QVariantMap>()){
                QVariantMap map = var.toMap();
                QVariant seqVar = map.value("seq");
                if(seqVar.canConvert<qint64>()){
                    qint64 seq = seqVar.toLongLong();
                    qDebug()<<"seq - "<<seq;
                    ret->setSeq(seq);
                }
                QVariant commandVar = map.value("command");
                if(commandVar.canConvert<QString>()){
                    QString command = commandVar.toString();
                    qDebug()<<"command - "<<command;
                    ret->setCommand(command);
                }
            }
        }
        return ret;
    }else {
        qDebug()<<"jsonError:"<<jsonError.errorString();
        return ret;
    }
}
