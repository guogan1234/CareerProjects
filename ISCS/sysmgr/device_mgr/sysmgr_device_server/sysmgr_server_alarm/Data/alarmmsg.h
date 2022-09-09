#ifndef ALARMMSG_H
#define ALARMMSG_H

struct AlarmMsg
{
    char label[128];
    long long time;
    int region_id;
    int station_id;
    int domain_id;
    char dev_type_label[16];
    char ctrl_trans_grp_label[32];
    bool is_restrain; //是否报警抑制(true 不抑制; false 抑制)
    char pic_name[64];
    int alarm_mode; //0 alarm; 1 rtn alarm;2 event only;  3 do nothing
    char content[256];//报警内容，最大256字节
    int source_id;//指定报警来源（可自定义以下报警配置），若无则填0
    int alarm_type;// 若有指定报警来源则填0
    int is_del_on_ack;//若有指定报警来源则填0
    bool is_replace_alarm;//若有指定报警来源则填false
    int alarm_level;//若有指定报警来源则
};

class rtdbAlarmMsg
{
public:
    rtdbAlarmMsg() {}

    int domainId;
    int type;
    char* msg;
};

#endif // ALARMMSG_H
