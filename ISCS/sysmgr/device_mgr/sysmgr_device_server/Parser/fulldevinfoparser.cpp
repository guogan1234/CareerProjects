#include "fulldevinfoparser.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QList>

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
