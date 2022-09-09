#ifndef CFDBSERVICE_H
#define CFDBSERVICE_H

#include <QObject>
#include "confcmn.h"
#include "configstruct.h"
#include "commnstruct.h"

class CfdbService : public QObject
{
    Q_OBJECT
public:
    explicit CfdbService(QObject *parent = nullptr);
    ~CfdbService();
    int setDataMap(int domianid, int type, void *data);
    int getDataMap(int domianid,int type,void *data);
    int setData(int domianid,int type,void *data);

private:
    int set_config_data(int domianid, ConfigData *data);
    int update_config_data(int domianid,ConfigData *data);
    int update_color_data(int domianid,AlarmColor *data);
    int get_config_data(int domianid,ConfigData *data);
    int get_color_data(int domianid,AlarmColor *data);

    int set_config_data(int domianid,QMap<int,ConfigData> *data);
    int update_config_data(int domianid,QMap<int,ConfigData> *data);
    int update_color_data(int domianid,QMap<int,AlarmColor> *data);
    int get_config_data(int domianid,QMap<int,ConfigData> *data);
    int get_color_data(int domianid,QMap<int,AlarmColor> *data);

    bool isValidDomain(int domainid);
    void setTestData(int num,QMap<int,ConfigData> &map);//test used;
    void setTestColor(int num,QMap<int,AlarmColor> &map);//test used;
    void setTestData(int num,QMap<int,ConfigData> *map);//test used;
    void setTestColor(int num,QMap<int,AlarmColor> *map);//test used;

private:
    QString m_qstrquery_alarmconfigtbl = QString("select * from conf.alarm_config_tbl");
    QString m_qstrquery_alarmleveltbl = QString("select * from conf.alarm_level_tbl");
    QString m_qstrinsert_alarmconfigtbl = QString("insert into conf.alarm_config_tbl (config_id,config_name,text_font,config_info) values('%1','%2','%3','%4')");
    QString m_qstrupdate_alarmconfigtbl = QString("UPDATE conf.alarm_config_tbl SET text_font = '%1' , config_info = '%2' WHERE config_id = '%3'");
    QString m_qstrupdate_alarmleveltbl = QString("UPDATE conf.alarm_level_tbl SET alarm_text_color = '%1' , alarm_background_color = '%2' , alarm_flash_text_color = '%3' , alarm_flash_background_color = '%4' ,"
                                               "recover_text_color = '%5' , recover_background_color = '%6' , recover_flash_text_color = '%7' , recover_flash_background_color = '%8' WHERE level_id = '%9'");
    QString m_qstrdelete_alarmconfigtbl = QString("TRUNCATE conf.alarm_config_tbl");
};

#endif // CFDBSERVICE_H
