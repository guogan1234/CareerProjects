#include "sysmgr_server_alarm.h"
#include "processor/alarmthread.h"
#include "Parser/alarmmsgparser.h"
#include "RTDBCmdMsgDef.h"
#include <QDateTime>
#include <QDebug>

int cpuAlarmCount = 0;
int memoryAlarmCount = 0;

int callerCount = 0;

Sysmgr_server_alarm::Sysmgr_server_alarm()
{
}

int Sysmgr_server_alarm::alarmHandle(AlarmConfig *config, AlarmExtra *extra, DevInfo *devInfo)
{
    if(!config || !devInfo || !extra){
        qDebug("alarmHandle params is invalid!\n");
        return -1;
    }
    limitCount = (config->sustainTime * 1000)/config->collectPeriod;
//    limitCount = 2;
    checkCpu(config->cpuLimit,extra,devInfo->cpuList);
    checkMem(config->memLimit,extra,devInfo->memory);
    checkDisk(config->diskLimit,extra,devInfo->diskList);

    return 0;
}

int Sysmgr_server_alarm::checkCaller(int limit, const char *key)
{
    if(!key){
        qDebug("param is NULL!\n");
        return -1;
    }
    qDebug("key - %s,callerCount - %d\n",key,callerCount);
    if(limit > 100){
        callerCount++;
        qDebug("key - %s,callerCount - %d\n",key,callerCount);
    }

    return 0;
}

int Sysmgr_server_alarm::checkCpu(int limit, AlarmExtra *extra, QList<Cpu *> *cpuList)
{
    if(!cpuList) return -1;
    qDebug("cpuLimit - %d,cpuAlarmCount - %d\n",limit,cpuAlarmCount);
    Cpu* cpu = cpuList->value(0);
    if(cpu){
//        qDebug("cpu name - %s\n",cpu->name);
//        limit = limit - 70;
        qDebug()<<"usedRate"<<cpu->usedRate<<"limit"<<limit;
        if(cpu->usedRate > limit){
            cpuAlarmCount++;
            qDebug("cpu alarm!cpuAlarmCount - %d\n",cpuAlarmCount);
            if(cpuAlarmCount >= limitCount){
                AlarmThread* alarmThread = AlarmThread::getInstance();
                if(!alarmThread->isRun()){
                    alarmThread->start();
                }
//                while (!alarmThread->isRun()) {
//                    printf("wait alarmThread start run!\n");
//                    qDebug("wait alarmThread start run!\n");
//                    continue;
//                }

                const char* cpuAlarm = "cpu告警";
                //rtdb json class
                AlarmMsg* alarmMsg = new AlarmMsg();
                long long time = QDateTime::currentMSecsSinceEpoch();
                time = time * 1000000;
                sprintf(alarmMsg->label,"%s_cpu",extra->nodeName);
                alarmMsg->time = time;
                alarmMsg->region_id = 1;
                alarmMsg->station_id = 1;
//                alarmMsg->station_id = extra->stationId;
                alarmMsg->domain_id = extra->domainId;
                sprintf(alarmMsg->dev_type_label,"%d",0);
                sprintf(alarmMsg->ctrl_trans_grp_label,"%d",0);
                alarmMsg->is_restrain = true;
                sprintf(alarmMsg->pic_name,"%d",0);
                alarmMsg->alarm_mode = 0;
                sprintf(alarmMsg->content,"%s %s",extra->nodeName,cpuAlarm);
                alarmMsg->source_id = 6;
                alarmMsg->alarm_type = 5;
                alarmMsg->is_del_on_ack = 1;
                alarmMsg->is_replace_alarm = true;
                alarmMsg->alarm_level = 3;

                //class to json string
                char* jsonMsg = AlarmMsgParser::classToJsonString(alarmMsg);

                rtdbAlarmMsg* rtdbAlarm = new rtdbAlarmMsg();
                rtdbAlarm->msg = strdup(jsonMsg);
                rtdbAlarm->domainId = extra->domainId;
                rtdbAlarm->type = MSGCMD_DEST_PLUGIN;

                alarmThread->setAlarmMsg(rtdbAlarm);

                //置0
                cpuAlarmCount = 0;
            }        
        }
        else{
            if(cpuAlarmCount != 0){
                AlarmThread* alarmThread = AlarmThread::getInstance();
                if(!alarmThread->isRun()){
                    alarmThread->start();
                }
                const char* cpuAlarm = "cpu告警恢复";
                //rtdb json class
                AlarmMsg* alarmMsg = new AlarmMsg();
                long long time = QDateTime::currentMSecsSinceEpoch();
                time = time * 1000000;
                sprintf(alarmMsg->label,"%s_cpu",extra->nodeName);
                alarmMsg->time = time;
                alarmMsg->region_id = 1;
                alarmMsg->station_id = 1;
//                alarmMsg->station_id = extra->stationId;
                alarmMsg->domain_id = extra->domainId;
                sprintf(alarmMsg->dev_type_label,"%d",0);
                sprintf(alarmMsg->ctrl_trans_grp_label,"%d",0);
                alarmMsg->is_restrain = true;
                sprintf(alarmMsg->pic_name,"%d",0);
                alarmMsg->alarm_mode = 1;
                sprintf(alarmMsg->content,"%s %s",extra->nodeName,cpuAlarm);
                alarmMsg->source_id = 6;
                alarmMsg->alarm_type = 5;
                alarmMsg->is_del_on_ack = 1;
                alarmMsg->is_replace_alarm = true;
                alarmMsg->alarm_level = 3;

                //class to json string
                char* jsonMsg = AlarmMsgParser::classToJsonString(alarmMsg);

                rtdbAlarmMsg* rtdbAlarm = new rtdbAlarmMsg();
                rtdbAlarm->msg = strdup(jsonMsg);
                rtdbAlarm->domainId = extra->domainId;
                rtdbAlarm->type = MSGCMD_DEST_PLUGIN;

                alarmThread->setAlarmMsg(rtdbAlarm);
            }
            cpuAlarmCount = 0;
        }
    }

    return 0;
}

int Sysmgr_server_alarm::checkMem(int limit, AlarmExtra *extra, Memory *memory)
{
    if(!memory || !extra) return -1;
    qDebug("memLimit - %d,memoryAlarmCount - %d\n",limit,memoryAlarmCount);
    long long used = (memory->used*100)/memory->total;
//    limit = limit - 70;
    qDebug()<<"used"<<used<<"limit"<<limit;
    if(used > limit){
        memoryAlarmCount++;
        qDebug("memory alarm!memoryAlarmCount - %d\n",memoryAlarmCount);
        if(memoryAlarmCount >= limitCount){
            //告警
            AlarmThread* alarmThread = AlarmThread::getInstance();
            if(!alarmThread->isRun()){
                alarmThread->start();
            }
            const char* memAlarm = "内存告警";
            //rtdb json class
            AlarmMsg* alarmMsg = new AlarmMsg();
            long long time = QDateTime::currentMSecsSinceEpoch();
            time = time * 1000000;
            sprintf(alarmMsg->label,"%s_memory",extra->nodeName);
            alarmMsg->time = time;
            alarmMsg->region_id = 1;
            alarmMsg->station_id = 1;
//            alarmMsg->station_id = extra->stationId;
            alarmMsg->domain_id = extra->domainId;
            sprintf(alarmMsg->dev_type_label,"%d",0);
            sprintf(alarmMsg->ctrl_trans_grp_label,"%d",0);
            alarmMsg->is_restrain = true;
            sprintf(alarmMsg->pic_name,"%d",0);
            alarmMsg->alarm_mode = 0;
            sprintf(alarmMsg->content,"%s %s",extra->nodeName,memAlarm);
            alarmMsg->source_id = 7;
            alarmMsg->alarm_type = 5;
            alarmMsg->is_del_on_ack = 1;
            alarmMsg->is_replace_alarm = true;
            alarmMsg->alarm_level = 3;
            //class to json string
            char* jsonMsg = AlarmMsgParser::classToJsonString(alarmMsg);

            rtdbAlarmMsg* rtdbAlarm = new rtdbAlarmMsg();
            rtdbAlarm->msg = strdup(jsonMsg);
            rtdbAlarm->domainId = extra->domainId;
            rtdbAlarm->type = MSGCMD_DEST_PLUGIN;

            alarmThread->setAlarmMsg(rtdbAlarm);

            //置0
            memoryAlarmCount = 0;
        }
    }
    else {
        if(!memoryAlarmCount){
            AlarmThread* alarmThread = AlarmThread::getInstance();
            if(!alarmThread->isRun()){
                alarmThread->start();
            }
            const char* memAlarm = "内存告警恢复";
            //rtdb json class
            AlarmMsg* alarmMsg = new AlarmMsg();
            long long time = QDateTime::currentMSecsSinceEpoch();
            time = time * 1000000;
            sprintf(alarmMsg->label,"%s_memory",extra->nodeName);
            alarmMsg->time = time;
            alarmMsg->region_id = 1;
            alarmMsg->station_id = 1;
//            alarmMsg->station_id = extra->stationId;
            alarmMsg->domain_id = extra->domainId;
            sprintf(alarmMsg->dev_type_label,"%d",0);
            sprintf(alarmMsg->ctrl_trans_grp_label,"%d",0);
            alarmMsg->is_restrain = true;
            sprintf(alarmMsg->pic_name,"%d",0);
            alarmMsg->alarm_mode = 1;
            sprintf(alarmMsg->content,"%s %s",extra->nodeName,memAlarm);
            alarmMsg->source_id = 7;
            alarmMsg->alarm_type = 5;
            alarmMsg->is_del_on_ack = 1;
            alarmMsg->is_replace_alarm = true;
            alarmMsg->alarm_level = 3;
            //class to json string
            char* jsonMsg = AlarmMsgParser::classToJsonString(alarmMsg);

            rtdbAlarmMsg* rtdbAlarm = new rtdbAlarmMsg();
            rtdbAlarm->msg = strdup(jsonMsg);
            rtdbAlarm->domainId = extra->domainId;
            rtdbAlarm->type = MSGCMD_DEST_PLUGIN;

            alarmThread->setAlarmMsg(rtdbAlarm);
        }
        memoryAlarmCount = 0;
    }

    return 0;
}

int Sysmgr_server_alarm::checkDisk(int limit, AlarmExtra *extra, QList<Disk *> *diskList)
{
    if(!diskList || !extra) return -1;
    foreach (Disk* disk, *diskList) {
        qDebug("limit - %d,usedRate - %f\n",limit,disk->usedRate);
        if(disk->usedRate >= limit){
            AlarmThread* alarmThread = AlarmThread::getInstance();
            if(!alarmThread->isRun()){
                alarmThread->start();
            }
            QByteArray bArray = disk->name.toLocal8Bit();
            const char* diskName = bArray.constData();
            const char* diskAlarm = "磁盘告警";
            //rtdb json class
            AlarmMsg* alarmMsg = new AlarmMsg();
            long long time = QDateTime::currentMSecsSinceEpoch();
            time = time * 1000000;
            sprintf(alarmMsg->label,"%s_disk",extra->nodeName);
            alarmMsg->time = time;
            alarmMsg->region_id = 1;
            alarmMsg->station_id = 1;
//            alarmMsg->station_id = extra->stationId;
            alarmMsg->domain_id = extra->domainId;
            sprintf(alarmMsg->dev_type_label,"%d",0);
            sprintf(alarmMsg->ctrl_trans_grp_label,"%d",0);
            alarmMsg->is_restrain = true;
            sprintf(alarmMsg->pic_name,"%d",0);
            alarmMsg->alarm_mode = 0;
            sprintf(alarmMsg->content,"%s %s %s",extra->nodeName,diskName,diskAlarm);
            alarmMsg->source_id = 8;
            alarmMsg->alarm_type = 5;
            alarmMsg->is_del_on_ack = 1;
            alarmMsg->is_replace_alarm = true;
            alarmMsg->alarm_level = 4;

            //class to json string
            char* jsonMsg = AlarmMsgParser::classToJsonString(alarmMsg);

            rtdbAlarmMsg* rtdbAlarm = new rtdbAlarmMsg();
            rtdbAlarm->msg = strdup(jsonMsg);
            rtdbAlarm->domainId = extra->domainId;
            rtdbAlarm->type = MSGCMD_DEST_PLUGIN;

            alarmThread->setAlarmMsg(rtdbAlarm);
        }
        else{
            AlarmThread* alarmThread = AlarmThread::getInstance();
            if(!alarmThread->isRun()){
                alarmThread->start();
            }
            QByteArray bArray = disk->name.toLocal8Bit();
            const char* diskName = bArray.constData();
            const char* diskAlarm = "磁盘告警恢复";
            //rtdb json class
            AlarmMsg* alarmMsg = new AlarmMsg();
            long long time = QDateTime::currentMSecsSinceEpoch();
            time = time * 1000000;
            sprintf(alarmMsg->label,"%s_disk",extra->nodeName);
            alarmMsg->time = time;
            alarmMsg->region_id = 1;
            alarmMsg->station_id = 1;
//            alarmMsg->station_id = extra->stationId;
            alarmMsg->domain_id = extra->domainId;
            sprintf(alarmMsg->dev_type_label,"%d",0);
            sprintf(alarmMsg->ctrl_trans_grp_label,"%d",0);
            alarmMsg->is_restrain = true;
            sprintf(alarmMsg->pic_name,"%d",0);
            alarmMsg->alarm_mode = 1;
            sprintf(alarmMsg->content,"%s %s %s",extra->nodeName,diskName,diskAlarm);
            alarmMsg->source_id = 8;
            alarmMsg->alarm_type = 5;
            alarmMsg->is_del_on_ack = 1;
            alarmMsg->is_replace_alarm = true;
            alarmMsg->alarm_level = 4;

            //class to json string
            char* jsonMsg = AlarmMsgParser::classToJsonString(alarmMsg);

            rtdbAlarmMsg* rtdbAlarm = new rtdbAlarmMsg();
            rtdbAlarm->msg = strdup(jsonMsg);
            rtdbAlarm->domainId = extra->domainId;
            rtdbAlarm->type = MSGCMD_DEST_PLUGIN;

            alarmThread->setAlarmMsg(rtdbAlarm);
        }
    }
    return 0;
}
