#include "devinfoparser.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

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

DevInfo *DevInfoParser::jsonToClass(const QByteArray &json)
{
    if(json.isEmpty() || json.isNull()) return NULL;
    QJsonParseError jsonError;
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(json,&jsonError);
    if(jsonError.error!=QJsonParseError::NoError || doc.isNull()){
        qDebug()<<"[Json Parse Error]"<<jsonError.errorString();
        return NULL;
    }
    DevInfo* dev = new DevInfo();
    jsonDocToFillClass(doc,dev);
    return dev;
}

void DevInfoParser::jsonDocToFillClass(const QJsonDocument &doc, DevInfo *dev)
{
    QVariant variant;
    variant = doc.toVariant();
    if(variant.canConvert<QVariantMap>()){
        QVariantMap map;
        map = variant.toMap();
        QVariant value;

        value = map.value("seq");
        if(value.canConvert<qlonglong>()){
            qlonglong seq = value.toLongLong();
            dev->seq = seq;
        }

        value = map.value("procList");
        QList<Process*> * procList = NULL;
        if(value.canConvert<QVariantList>()){
            QVariantList list;
            list = value.toList();
            procList = new QList<Process*>();
            for(int i = 0;i < list.size(); ++i){
                QVariant var;
                var = list.at(i);
                Process* process = new Process();
                if(var.canConvert<QVariantMap>()){
                    QVariantMap varProcess;
                    varProcess = var.toMap();
                    QVariant varName = varProcess.value("name");
                    if(varName.canConvert<QString>()){
                        QString name = varName.toString();
                        process->name = name;
                    }
                    QVariant varCpuRate = varProcess.value("cpuRate");
                    if(varCpuRate.canConvert<double>()){
                        double cpuRate = varCpuRate.toDouble();
                        process->cpuRate = cpuRate;
                    }
                    QVariant varMemUsed = varProcess.value("memUsed");
                    if(varMemUsed.canConvert<qlonglong>()){
                        qlonglong memUsed = varMemUsed.toLongLong();
                        process->memUsed = memUsed;
                    }
                    QVariant varRun = varProcess.value("bRun");
                    if(varRun.canConvert<int>()){
                        int bRun = varRun.toInt();
                        process->bRun = bRun;
                    }
                }
                procList->append(process);
            }
        }

        value = map.value("cpuList");
        QList<Cpu*> * cpuList = NULL;
        if(value.canConvert<QVariantList>()){
            QVariantList list;
            list = value.toList();
            cpuList = new QList<Cpu*>();
            for(int i = 0;i < cpuList->size(); ++i){
                QVariant varCpu;
                varCpu = list.at(i);
                Cpu* cpu = new Cpu();
                if(varCpu.canConvert<QVariantMap>()){
                    QVariantMap varCpuMap;
                    varCpuMap = varCpu.toMap();
                    QVariant varName = varCpuMap.value("name");
                    if(varName.canConvert<QString>()){
                        QString name = varName.toString();
                        cpu->name = name;
                    }
                    QVariant varUsedRate = varCpuMap.value("usedRate");
                    if(varUsedRate.canConvert<double>()){
                        double usedRate = varUsedRate.toDouble();
                        cpu->usedRate = usedRate;
                    }
                }
                cpuList->append(cpu);
            }
        }

        value = map.value("memory");
        Memory* memory = new Memory();
        if(value.canConvert<QVariantMap>()){
            QVariantMap varMemory = value.toMap();
            QVariant varTotal = varMemory.value("total");
            if(varTotal.canConvert<qlonglong>()){
                qlonglong total = varTotal.toLongLong();
                memory->total = total;
            }
            QVariant varFree = varMemory.value("free");
            if(varFree.canConvert<qlonglong>()){
                qlonglong free = varFree.toLongLong();
                memory->free = free;
            }
            QVariant varUsed = varMemory.value("used");
            if(varUsed.canConvert<qlonglong>()){
                qlonglong used = varUsed.toLongLong();
                memory->used = used;
            }
        }

        value = map.value("diskList");
        QList<Disk*> * diskList = NULL;
        if(value.canConvert<QVariantList>()){
            QVariantList list = value.toList();
            diskList = new QList<Disk*>();
            for(int i = 0;i < list.size(); ++i){
                QVariant varDisk = list.at(i);
                Disk* disk = new Disk();
                if(varDisk.canConvert<QVariantMap>()){
                    QVariantMap varDiskMap = varDisk.toMap();
                    QVariant varName = varDiskMap.value("name");
                    if(varName.canConvert<QString>()){
                        QString name = varName.toString();
                        disk->name = name;
                    }
                    QVariant varTotal = varDiskMap.value("total");
                    if(varTotal.canConvert<qlonglong>()){
                        qlonglong total = varTotal.toLongLong();
                        disk->total = total;
                    }
                    QVariant varUsed = varDiskMap.value("used");
                    if(varUsed.canConvert<qlonglong>()){
                        qlonglong used = varUsed.toLongLong();
                        disk->used = used;
                    }
                    QVariant varFree = varDiskMap.value("free");
                    if(varFree.canConvert<qlonglong>()){
                        qlonglong free = varFree.toLongLong();
                        disk->free = free;
                    }
                    QVariant varUsedRate = varDiskMap.value("usedRate");
                    if(varUsedRate.canConvert<double>()){
                        double usedRate = varUsedRate.toDouble();
                        disk->usedRate = usedRate;
                    }
                }
                diskList->append(disk);
            }
        }

        value = map.value("timestamp");
        if(value.canConvert<long long>()){
            long long timestamp = value.toLongLong();
            dev->timestamp = timestamp;
        }

        dev->procList = procList;
        dev->cpuList = cpuList;
        dev->memory = memory;
        dev->diskList = diskList;
    }
}

void DevInfoParser::changeByteArrayToClass(const QByteArray &json, DevInfo *info)
{
    if(!info) return;
    QJsonParseError jsonError;
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(json,&jsonError);
    if(jsonError.error!=QJsonParseError::NoError || doc.isNull()){
        qDebug()<<"[Json Parse Error]"<<jsonError.errorString();
        return;
    }
    jsonDocToFillClass(doc,info);
}
