#include "fulldevinfoparser.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QList>
#include <QDebug>

FullDevInfoParser::FullDevInfoParser()
{

}

QByteArray FullDevInfoParser::jsonClassToByteArray(FullDevInfo *object)
{
    QByteArray bArray;
    if(!object) return bArray;

    DevInfo* dev = object->devInfo;
    NetInfo* net = object->netInfo;
    QJsonObject jsonNet;
    QJsonObject jsonDev;
    if(net){
        QString name = net->name;
//        bool bConn = net->bConn;
        QList<NetConn*> * netConnList = net->netConn;
        QJsonArray netConnArray;
        for(int i = 0;i < netConnList->size();i++){
            NetConn* netConn = netConnList->at(i);
            QJsonObject jsonNetConn;
            jsonNetConn.insert("ip",QJsonValue(netConn->ip));
            jsonNetConn.insert("bConn",QJsonValue(netConn->bConn));
            netConnArray.append(QJsonValue(jsonNetConn));
        }
        jsonNet.insert("name",QJsonValue(name));
//        jsonNet.insert("bConn",QJsonValue(bConn));
        jsonNet.insert("netConn",QJsonValue(netConnArray));
    }
    if(dev){
        long long seq = dev->seq;
        jsonDev.insert("seq",QJsonValue(seq));
        QList<Process*> * procList = dev->procList;
        QJsonArray procArray;
        if(procList){
            for(int i = 0;i < procList->size(); ++i){
                Process* process = procList->at(i);
                QJsonObject jsonProc;
                if(process){
                    QString name = process->name;
                    double cpuRate = process->cpuRate;
                    long long memUsed = process->memUsed;
                    bool bRun = process->bRun;
                    jsonProc.insert("name",QJsonValue(name));
                    jsonProc.insert("cpuRate",QJsonValue(cpuRate));
                    jsonProc.insert("memUsed",QJsonValue(memUsed));
                    jsonProc.insert("bRun",QJsonValue(bRun));
                }
                procArray.append(QJsonValue(jsonProc));
            }
        }
        jsonDev.insert("procList",QJsonValue(procArray));
        QList<Cpu*> * cpuList = dev->cpuList;
        QJsonArray cpuArray;
        if(cpuList){
            for(int i = 0;i < cpuList->size(); ++i){
                Cpu* cpu = cpuList->at(i);
                QJsonObject jsonCpu;
                if(cpu){
                    QString name = cpu->name;
                    double usedRate = cpu->usedRate;
                    jsonCpu.insert("name",QJsonValue(name));
                    jsonCpu.insert("usedRate",QJsonValue(usedRate));
                }
                cpuArray.append(QJsonValue(jsonCpu));
            }
        }
        jsonDev.insert("cpuList",QJsonValue(cpuArray));
        Memory* memory = dev->memory;
        QJsonObject mem;
        if(memory){
            long long free = memory->free;
            long long total = memory->total;
            long long used = memory->used;
            mem.insert("free",QJsonValue(free));
            mem.insert("total",QJsonValue(total));
            mem.insert("used",QJsonValue(used));
        }
        jsonDev.insert("memory",QJsonValue(mem));
        QList<Disk*> * diskList = dev->diskList;
        QJsonArray diskArray;
        if(diskList){
            for(int i = 0;i < diskList->size(); ++i){
                Disk* disk = diskList->at(i);
                QJsonObject jsonDisk;
                if(disk){
                    long long free = disk->free;
                    QString name = disk->name;
                    long long total = disk->total;
                    long long used = disk->used;
                    double usedRate = disk->usedRate;
                    jsonDisk.insert("free",QJsonValue(free));
                    jsonDisk.insert("name",QJsonValue(name));
                    jsonDisk.insert("total",QJsonValue(total));
                    jsonDisk.insert("used",QJsonValue(used));
                    jsonDisk.insert("usedRate",QJsonValue(usedRate));
                }
                diskArray.append(QJsonValue(jsonDisk));
            }
        }
        jsonDev.insert("diskList",QJsonValue(diskArray));
        long long timestamp = dev->timestamp;
        jsonDev.insert("timestamp",QJsonValue(timestamp));
    }
    QJsonObject root;
    root.insert("devInfo",QJsonValue(jsonDev));
    root.insert("netInfo",QJsonValue(jsonNet));
    QJsonDocument doc;
    doc.setObject(root);
    return doc.toJson(QJsonDocument::Compact);
}

FullDevInfo *FullDevInfoParser::byteArrayToJsonClass(QByteArray bArray)
{
    if(bArray.isEmpty() || bArray.isNull()) return NULL;

    QJsonParseError jsonError;
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(bArray,&jsonError);
    if(jsonError.error!=QJsonParseError::NoError || doc.isNull()){
        qDebug()<<"[Json Parse Error]"<<jsonError.errorString();
        return NULL;
    }
    FullDevInfo* info = new FullDevInfo();
    jsonDocToFillClass(doc,info);
    return info;
}

void FullDevInfoParser::changeByteArrayToClass(QByteArray bArray, FullDevInfo *info)
{
    if(bArray.isEmpty() || bArray.isNull()) return;
    if(!info) return;
    QJsonParseError jsonError;
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(bArray,&jsonError);
    if(jsonError.error!=QJsonParseError::NoError || doc.isNull()){
        qDebug()<<"[Json Parse Error]"<<jsonError.errorString();
        return;
    }
    jsonDocToFillClass(doc,info);
}

void FullDevInfoParser::jsonDocToFillClass(const QJsonDocument &doc, FullDevInfo *info)
{
    if(doc.isNull()) return;
    QVariant varObject = doc.toVariant();
    if(varObject.canConvert<QVariantMap>()){
        QVariantMap varMap = varObject.toMap();
        QVariant varDev = varMap.value("devInfo");
        if(varDev.canConvert<QVariantMap>()){
            QVariantMap map = varDev.toMap();
            DevInfo* dev = new DevInfo();
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
                for(int i = 0;i < list.size(); ++i){
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

            info->devInfo = dev;
        }
        QVariant varNet = varMap.value("netInfo");
        if(varNet.canConvert<QVariantMap>()){
            QVariantMap varNetMap = varNet.toMap();
            NetInfo* net = new NetInfo();
            QVariant varName = varNetMap.value("name");
            if(varName.canConvert<QString>()){
                net->name = varName.toString();
            }
//            QVariant varConn = varNetMap.value("bConn");
//            if(varConn.canConvert<bool>()){
//                net->bConn = varConn.toBool();
//            }
            net->netConn = new QList<NetConn*>();
            QVariant varNetConn = varNetMap.value("netConn");
            if(varNetConn.canConvert<QVariantList>()){
                QVariantList varNetConnList = varNetConn.toList();
                for(int i = 0;i < varNetConnList.size();i++){
                    QVariant temp = varNetConnList.at(i);
                    NetConn* tempObj = new NetConn();
                    if(temp.canConvert<QVariantMap>()){
                        QVariantMap varNetConnMap = temp.toMap();
                        tempObj->ip = varNetConnMap.value("ip").toString();
                        tempObj->bConn = varNetConnMap.value("bConn").toBool();
                        net->netConn->append(tempObj);
                    }
                }
            }
            info->netInfo = net;
        }
    }
}
