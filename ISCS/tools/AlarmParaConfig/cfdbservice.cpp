#include "cfdbservice.h"
#include "translateservice.h"
#include <QDebug>

CfdbService::CfdbService(QObject *parent) : QObject(parent)
{

}

CfdbService::~CfdbService()
{

}

int CfdbService::setDataMap(int domianid, int type, void *data)
{
    if(!isValidDomain(domianid))
        return -1;
    QMap<int,ConfigData> *pconfigdata_temp = NULL;
    QMap<int,AlarmColor> *palarmcolor_temp = NULL;

    switch (type) {
    case NODE_MANAGER:
        pconfigdata_temp = (QMap<int,ConfigData> *)data;
        if(!pconfigdata_temp)
            return -1;
        return set_config_data(domianid,pconfigdata_temp);
    case NODE_CONFIG:
        pconfigdata_temp = (QMap<int,ConfigData> *)data;
        if(!pconfigdata_temp)
            return -1;
        return update_config_data(domianid,pconfigdata_temp);
    case COLOR_CONFIG:
        palarmcolor_temp = (QMap<int,AlarmColor> *)data;
        if(!palarmcolor_temp)
            return -1;
        return update_color_data(domianid,palarmcolor_temp);
    default:
        break;
    }
    return 0;
}

int CfdbService::getDataMap(int domianid, int type, void *data)
{
    if(!isValidDomain(domianid))
        return -1;
    QMap<int,ConfigData> *pconfigdata_temp = NULL;
    QMap<int,AlarmColor> *palarmcolor_temp = NULL;
    switch (type) {
    case NODE_MANAGER:
    case NODE_CONFIG:
        pconfigdata_temp = (QMap<int,ConfigData> *)data;
        if(!pconfigdata_temp)
            return -1;
        return get_config_data(domianid,pconfigdata_temp);
    case COLOR_CONFIG:
        palarmcolor_temp = (QMap<int,AlarmColor> *)data;
        if(!palarmcolor_temp)
            return -1;
        return get_color_data(domianid,palarmcolor_temp);
    default:
        break;
    }
    return 0;
}

int CfdbService::setData(int domianid, int type, void *data)
{
    if(!isValidDomain(domianid))
        return -1;
    ConfigData *pconfigdata_temp = NULL;
    AlarmColor *palarmcolor_temp = NULL;

    switch (type) {
    case NODE_MANAGER:
        pconfigdata_temp = (ConfigData *)data;
        if(!pconfigdata_temp)
            return -1;
        return set_config_data(domianid,pconfigdata_temp);
    case NODE_CONFIG:
        pconfigdata_temp = (ConfigData *)data;
        if(!pconfigdata_temp)
            return -1;
        return update_config_data(domianid,pconfigdata_temp);
    case COLOR_CONFIG:
        palarmcolor_temp = (AlarmColor *)data;
        if(!palarmcolor_temp)
            return -1;
        return update_color_data(domianid,palarmcolor_temp);
    default:
        break;
    }
    return 0;
}

int CfdbService::set_config_data(int domianid, QMap<int, ConfigData> *data)
{
//    confdb_init();
    confdb_exec(m_qstrdelete_alarmconfigtbl.toStdString().c_str());
    QMap<int, ConfigData>::iterator iterator_config = data->begin();
    while(data->end() != iterator_config)
    {
        QString qstr_config_id = QString::number(iterator_config.value().m_nconfig_id);
        QString qstr_config_name = iterator_config.value().m_qstr_config_name;
        QString qstr_text_font = iterator_config.value().m_qstr_text_font;
        TransLateService translateservice;
        QString qstr_config_info = translateservice.TransLateMapToJson((*data)[iterator_config.key()].m_qmap_fields,(*data)[iterator_config.key()].m_qmap_policy);

        if(qstr_config_name.isEmpty())
            qstr_config_name.append("null");
        if(qstr_text_font.isEmpty())
            qstr_text_font.append("null");
        if(qstr_config_info.isEmpty())
            qstr_config_info.append("null");
        QString qstr_condition;
        qstr_condition.append(this->m_qstrinsert_alarmconfigtbl.arg(qstr_config_id).arg(qstr_config_name).arg(qstr_text_font).arg(qstr_config_info));
        confdb_exec(qstr_condition.toStdString().c_str());
        ++iterator_config;
    }
//    confdb_release();
}

int CfdbService::update_color_data(int domianid, QMap<int, AlarmColor> *data)
{
    QMap<int, AlarmColor>::iterator iterator_color = data->begin();
    while(data->end() != iterator_color)
    {
        QString qstr_condition;
        qstr_condition.append(this->m_qstrupdate_alarmleveltbl.arg(iterator_color.value().m_qstr_alarm_text).arg(iterator_color.value().m_qstr_alarm_back)
                              .arg(iterator_color.value().m_qstr_alarm_flash_text).arg(iterator_color.value().m_qstr_alarm_flash_back)
                              .arg(iterator_color.value().m_qstr_recover_text).arg(iterator_color.value().m_qstr_recover_back)
                              .arg(iterator_color.value().m_qstr_recover_flash_text).arg(iterator_color.value().m_qstr_recover_flash_back)
                              .arg(iterator_color.value().m_nalarm_level));
        confdb_exec(qstr_condition.toStdString().c_str());
        ++iterator_color;
    }
}

int CfdbService::get_config_data(int domianid, QMap<int, ConfigData> *data)
{
    confdb_init();
    ConfDbRecordSet *cfdb_records = NULL;
    cfdb_records = confdb_query("select * from conf.alarm_config_tbl");
    if(cfdb_records)
    {
        int nrows = confdb_recordset_get_count(cfdb_records);
        AlarmConfigTbl configinfotbl;
        for(int i = 0;nrows > i;i++)
        {
            ConfigData configdata;
            for(int j = 0;j < configinfotbl.Return_FieldsSize();j++)
            {
                const char *ch_value = confdb_recordset_get(cfdb_records,i,configinfotbl.Return_FieldsName(j));
                if(!ch_value)
                    continue;

                if(QString("config_id") == QString(configinfotbl.Return_FieldsName(j)))
                    configdata.m_nconfig_id = QString(ch_value).toInt();
                else if(QString("config_name") == QString(configinfotbl.Return_FieldsName(j)))
                    configdata.m_qstr_config_name = QString(ch_value);
                else if(QString("text_font") == QString(configinfotbl.Return_FieldsName(j)))
                    configdata.m_qstr_text_font = QString(ch_value);
                else if(QString("config_info") == QString(configinfotbl.Return_FieldsName(j)))
                {
                    configdata.m_qstr_config_info = QString(ch_value);
                    TransLateService translateservice;
                    translateservice.TransLateJsonToMap(configdata.m_qstr_config_info,configdata.m_qmap_fields,configdata.m_qmap_policy);
                }
                ch_value = NULL;
            }
            data->insert(i,configdata);
        }
    }

    confdb_recordset_free(cfdb_records);
//    confdb_release();
    return 0;
}

int CfdbService::get_color_data(int domianid, QMap<int, AlarmColor> *data)
{
    ConfDbRecordSet *cfdb_records = NULL;
    cfdb_records = confdb_query("select * from conf.alarm_level_tbl");
    if(cfdb_records)
    {
        int nrows = confdb_recordset_get_count(cfdb_records);
        AlarmLevelTbl alarmleveltbl;
        for(int i = 0;nrows > i;i++)
        {
            AlarmColor colordata;
            for(int j = 0;j < alarmleveltbl.get_fields_size();j++)
            {
                const char *ch_value = confdb_recordset_get(cfdb_records,i,alarmleveltbl.get_field_name(j));
                if(!ch_value)
                    continue;
                if(QString("level_id") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_nalarm_level = QString(ch_value).toInt();
                else if(QString("level_name") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_alarm_name = QString(ch_value);
                else if(QString("alarm_text_color") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_alarm_text = QString(ch_value);
                else if(QString("alarm_background_color") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_alarm_back = QString(ch_value);
                else if(QString("alarm_flash_text_color") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_alarm_flash_text = QString(ch_value);
                else if(QString("alarm_flash_background_color") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_alarm_flash_back = QString(ch_value);
                else if(QString("recover_text_color") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_recover_text = QString(ch_value);
                else if(QString("recover_background_color") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_recover_back = QString(ch_value);
                else if(QString("recover_flash_text_color") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_recover_flash_text = QString(ch_value);
                else if(QString("recover_flash_background_color") == QString(alarmleveltbl.get_field_name(j)))
                    colordata.m_qstr_recover_flash_back = QString(ch_value);
                ch_value = NULL;
            }
            data->insert(i,colordata);
        }
    }

    confdb_recordset_free(cfdb_records);
//    confdb_release();
    return 0;
}

int CfdbService::set_config_data(int domianid, ConfigData *data)
{
    return 0;
}

int CfdbService::update_config_data(int domianid, ConfigData *data)
{
    qDebug() << "update_config_data";
    QString qstr_condition;
    TransLateService transservice;
    data->m_qstr_config_info = transservice.TransLateMapToJson(data->m_qmap_fields,data->m_qmap_policy);
    qstr_condition.append(this->m_qstrupdate_alarmconfigtbl.arg(data->m_qstr_text_font).arg(data->m_qstr_config_info).arg(data->m_nconfig_id));
    qDebug() << qstr_condition;
    confdb_exec(qstr_condition.toStdString().c_str());
    return 0;
}

int CfdbService::update_color_data(int domianid, AlarmColor *data)
{
    qDebug() << "update_color_data";
    QString qstr_condition;
    qstr_condition.append(this->m_qstrupdate_alarmleveltbl.arg(data->m_qstr_alarm_text).arg(data->m_qstr_alarm_back)
                          .arg(data->m_qstr_alarm_flash_text).arg(data->m_qstr_alarm_flash_back)
                          .arg(data->m_qstr_recover_text).arg(data->m_qstr_recover_back)
                          .arg(data->m_qstr_recover_flash_text).arg(data->m_qstr_recover_flash_back)
                          .arg(data->m_nalarm_level));
    confdb_exec(qstr_condition.toStdString().c_str());
    return 0;
}

int CfdbService::get_config_data(int domianid, ConfigData *data)
{
    return 0;
}

int CfdbService::get_color_data(int domianid, AlarmColor *data)
{
    return 0;
}

int CfdbService::update_config_data(int domianid, QMap<int, ConfigData> *data)
{

}

bool CfdbService::isValidDomain(int domainid)
{
    if(0 >= domainid)
        return false;
    return true;
}

void CfdbService::setTestData(int num, QMap<int, ConfigData> &map)
{
    ConfigData configdata_temp;
    configdata_temp.m_nconfig_id = num;
    configdata_temp.m_qstr_config_name = QString("window%1").arg(num);
    configdata_temp.m_qstr_text_font = QString("font%1").arg(num);;
    for(int i = 0;i < 10 + num;i++)
    {
        configdata_temp.m_qmap_fields.insert(i,QString("field%1").arg(i));
        configdata_temp.m_qmap_policy.insert(i,qrand()%3);
    }
    map.insert(num,configdata_temp);
    return;
}

void CfdbService::setTestColor(int num, QMap<int, AlarmColor> &map)
{
    AlarmColor alarmcolor_temp;
    QString qstr_num = QString::number(qrand() % 9);
    alarmcolor_temp.m_nalarm_level = num;
    alarmcolor_temp.m_qstr_alarm_name = QString("level%1").arg(num);
    alarmcolor_temp.m_qstr_alarm_text = QString("#F%100000").arg(qstr_num);
    alarmcolor_temp.m_qstr_alarm_back = QString("#000000");
    alarmcolor_temp.m_qstr_alarm_flash_text = QString("#00000F%1").arg(qstr_num);
    alarmcolor_temp.m_qstr_alarm_flash_back = QString("#000000");
    alarmcolor_temp.m_qstr_recover_text = QString("#00F%1000").arg(qstr_num);
    alarmcolor_temp.m_qstr_recover_back = QString("#000000");
    alarmcolor_temp.m_qstr_recover_flash_text = QString("#0000F%10").arg(qstr_num);
    alarmcolor_temp.m_qstr_recover_flash_back = QString("#000000");
    map.insert(num,alarmcolor_temp);
}

void CfdbService::setTestData(int num, QMap<int, ConfigData> *map)
{
    ConfigData configdata_temp;
    configdata_temp.m_nconfig_id = num;
    configdata_temp.m_qstr_config_name = QString("window%1").arg(num);
    configdata_temp.m_qstr_text_font = QString("font%1").arg(num);;
    for(int i = 0;i < 10 + num;i++)
    {
        configdata_temp.m_qmap_fields.insert(i,QString("field%1").arg(i));
        configdata_temp.m_qmap_policy.insert(i,qrand()%3);
    }
    map->insert(num,configdata_temp);
    return;
}

void CfdbService::setTestColor(int num, QMap<int, AlarmColor> *map)
{
    AlarmColor alarmcolor_temp;
    QString qstr_num = QString::number(qrand() % 9);
    alarmcolor_temp.m_nalarm_level = num;
    alarmcolor_temp.m_qstr_alarm_name = QString("level%1").arg(num);
    alarmcolor_temp.m_qstr_alarm_text = QString("#F%100000").arg(qstr_num);
    alarmcolor_temp.m_qstr_alarm_back = QString("#000000");
    alarmcolor_temp.m_qstr_alarm_flash_text = QString("#00000F%1").arg(qstr_num);
    alarmcolor_temp.m_qstr_alarm_flash_back = QString("#000000");
    alarmcolor_temp.m_qstr_recover_text = QString("#00F%1000").arg(qstr_num);
    alarmcolor_temp.m_qstr_recover_back = QString("#000000");
    alarmcolor_temp.m_qstr_recover_flash_text = QString("#0000F%10").arg(qstr_num);
    alarmcolor_temp.m_qstr_recover_flash_back = QString("#000000");
    map->insert(num,alarmcolor_temp);
}
