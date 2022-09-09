#include "devinfoparser.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

DevInfoParser::DevInfoParser()
{

}

QByteArray DevInfoParser::jsonClassToByteArray(DevInfo *info)
{
    QByteArray bArray;
    if(!info) return bArray;
    QList<Process*> * processList = info->procList;
    QList<Cpu*> * cpuList = info->cpuList;
    Memory* memory = info->memory;
    QList<Disk*> * diskList = info->diskList;

    QJsonDocument doc;
    QJsonArray processArray;
    for(int i = 0;i < processList->size(); ++i){
        Process* value = processList->at(i);
        if(value){
            QJsonObject temp;
            temp.insert("name",QJsonValue(value->name));
            temp.insert("cpuRate",QJsonValue(value->cpuRate));
            temp.insert("memUsed",QJsonValue(value->memUsed));
            temp.insert("bRun",QJsonValue(value->bRun));

            processArray.append(QJsonValue(temp));
        }
    }
    QJsonArray cpuArray;
    for(int i = 0;i < cpuList->size(); ++i){
        Cpu* value = cpuList->at(i);
        if(value){
            QJsonObject temp;
            temp.insert("name",QJsonValue(value->name));
            temp.insert("usedRate",QJsonValue(value->usedRate));

            cpuArray.append(QJsonValue(temp));
        }
    }
    QJsonObject mem;
    if(memory){
        mem.insert("total",QJsonValue(memory->total));
        mem.insert("free",QJsonValue(memory->free));
        mem.insert("used",QJsonValue(memory->used));
    }
    QJsonArray diskArray;
    for(int i = 0;i < diskList->size(); ++i){
        Disk* value = diskList->at(i);
        if(value){
            QJsonObject temp;
            temp.insert("name",QJsonValue(value->name));
            temp.insert("total",QJsonValue(value->total));
            temp.insert("used",QJsonValue(value->used));
            temp.insert("free",QJsonValue(value->free));
            temp.insert("usedRate",QJsonValue(value->usedRate));

            diskArray.append(QJsonValue(temp));
        }
    }
    QJsonObject docRoot;
    docRoot.insert("seq",QJsonValue(info->seq));
    docRoot.insert("procList",QJsonValue(processArray));
    docRoot.insert("cpuList",QJsonValue(cpuArray));
    docRoot.insert("memory",QJsonValue(mem));
    docRoot.insert("diskList",QJsonValue(diskArray));
    docRoot.insert("timestamp",QJsonValue(info->timestamp));
    doc.setObject(docRoot);

    return doc.toJson(QJsonDocument::Compact);
}
