#include "configworkerimpl.h"
#include "Parser/alarmconfigparser.h"
#include "configmanager.h"
#include <QDebug>

ConfigWorkerImpl::ConfigWorkerImpl(QObject *parent) : QObject(parent)
{

}

void ConfigWorkerImpl::dataIncome(QByteArray datas)
{
    qDebug()<<"[ConfigWorkerImpl]"<<datas;
    QList<KeyValue*> * configs  = NULL;
    configs = AlarmConfigParser::jsonBytesToClass(datas);
    ConfigManager* configManager = new ConfigManager();
    configManager->saveConf(configs);
}
