#include "commnstruct.h"

EventInfoTbl:: EventInfoTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("label");
    this->m_qlist_fieldsname.append("second");
    this->m_qlist_fieldsname.append("msecond");
    this->m_qlist_fieldsname.append("alarm_type");
    this->m_qlist_fieldsname.append("domain_id");
    this->m_qlist_fieldsname.append("station_id");
    this->m_qlist_fieldsname.append("prosystem_id");
    this->m_qlist_fieldsname.append("region_id");
    this->m_qlist_fieldsname.append("dev_type_label");
    this->m_qlist_fieldsname.append("ack_userid");
    this->m_qlist_fieldsname.append("ack_time");
    this->m_qlist_fieldsname.append("ack_hostname");
    this->m_qlist_fieldsname.append("is_recovery");
    this->m_qlist_fieldsname.append("alarm_level");
    this->m_qlist_fieldsname.append("content");
}

EventInfoTbl::~EventInfoTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *EventInfoTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index )
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int EventInfoTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

DomainTbl::DomainTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("domain_id");
    this->m_qlist_fieldsname.append("domain_label");
    this->m_qlist_fieldsname.append("domain_name");
//    this->m_qlist_fieldsname.append("domain_type");
}

DomainTbl::~DomainTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *DomainTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int DomainTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

StationTbl::StationTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("station_id");
//    this->m_qlist_fieldsname.append("station_label");
    this->m_qlist_fieldsname.append("station_name");
    this->m_qlist_fieldsname.append("station_pid");
//    this->m_qlist_fieldsname.append("domian_id");
}

StationTbl::~StationTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *StationTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int StationTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

ProSystemTbl::ProSystemTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("pro_system_id");
//    this->m_qlist_fieldsname.append("pro_system_label");
    this->m_qlist_fieldsname.append("pro_system_name");
}

ProSystemTbl::~ProSystemTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *ProSystemTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int ProSystemTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

RegionTbl::RegionTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("region_id");
    this->m_qlist_fieldsname.append("pro_system_id");
    this->m_qlist_fieldsname.append("region_label");
    this->m_qlist_fieldsname.append("region_name");
}

RegionTbl::~RegionTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *RegionTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int RegionTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

DevTypeTbl::DevTypeTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("dev_type_label");
    this->m_qlist_fieldsname.append("dev_type_name");
    this->m_qlist_fieldsname.append("pro_system_id");
}

DevTypeTbl::~DevTypeTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *DevTypeTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int DevTypeTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

AlarmLevelTbl::AlarmLevelTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("level_id");
    this->m_qlist_fieldsname.append("level_name");
    this->m_qlist_fieldsname.append("priority");
    this->m_qlist_fieldsname.append("sound_file");
    this->m_qlist_fieldsname.append("alarm_text_color");
    this->m_qlist_fieldsname.append("alarm_background_color");
    this->m_qlist_fieldsname.append("alarm_flash_text_color");
    this->m_qlist_fieldsname.append("alarm_flash_background_color");
    this->m_qlist_fieldsname.append("recover_text_color");
    this->m_qlist_fieldsname.append("recover_background_color");
    this->m_qlist_fieldsname.append("recover_flash_text_color");
    this->m_qlist_fieldsname.append("recover_flash_background_color");
}

AlarmLevelTbl::~AlarmLevelTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *AlarmLevelTbl::get_field_name(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int AlarmLevelTbl::get_fields_size()
{
    return this->m_qlist_fieldsname.size();
}

AlarmConfigTbl::AlarmConfigTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("config_id");
    this->m_qlist_fieldsname.append("config_name");
    this->m_qlist_fieldsname.append("text_font");
    this->m_qlist_fieldsname.append("config_info");
}

AlarmConfigTbl::~AlarmConfigTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *AlarmConfigTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int AlarmConfigTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

AlarmTypeTbl::AlarmTypeTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("type_id");
    this->m_qlist_fieldsname.append("type_name");
}

AlarmTypeTbl::~AlarmTypeTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *AlarmTypeTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int AlarmTypeTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

PermGroupDefTbl::PermGroupDefTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("group_id");
    this->m_qlist_fieldsname.append("group_name");
}

PermGroupDefTbl::~PermGroupDefTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *PermGroupDefTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int PermGroupDefTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

PermUserDefTbl::PermUserDefTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("user_id");
    this->m_qlist_fieldsname.append("user_name");
}

PermUserDefTbl::~PermUserDefTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *PermUserDefTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int PermUserDefTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

PermUserGroupDefTbl::PermUserGroupDefTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("user_id");
    this->m_qlist_fieldsname.append("group_id");
}

PermUserGroupDefTbl::~PermUserGroupDefTbl()
{
    for(auto it_list : this->m_qlist_fieldsname)
    {
        delete it_list;
    }
    this->m_qlist_fieldsname.clear();
}

char *PermUserGroupDefTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int PermUserGroupDefTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}

AlarmInfoTbl::AlarmInfoTbl()
{
    this->m_qlist_fieldsname.clear();
    this->m_qlist_fieldsname.append("label");
    this->m_qlist_fieldsname.append("second");
//    this->m_qlist_fieldsname.append("msecond");
    this->m_qlist_fieldsname.append("alarm_type");
    this->m_qlist_fieldsname.append("domain_id");
    this->m_qlist_fieldsname.append("station_id");
    this->m_qlist_fieldsname.append("region_id");
    this->m_qlist_fieldsname.append("dev_type_label");
    this->m_qlist_fieldsname.append("is_restrain");
    this->m_qlist_fieldsname.append("pic_name");
    this->m_qlist_fieldsname.append("is_replace_alarm");
    this->m_qlist_fieldsname.append("is_recovery");
    this->m_qlist_fieldsname.append("alarm_level");
    this->m_qlist_fieldsname.append("content");
    this->m_qlist_fieldsname.append("is_acked");
}

AlarmInfoTbl::~AlarmInfoTbl()
{

}

char *AlarmInfoTbl::Return_FieldsName(const int index)
{
    if(0 > index || this->m_qlist_fieldsname.size() < index)
        return NULL;
    return this->m_qlist_fieldsname.at(index);
}

int AlarmInfoTbl::Return_FieldsSize()
{
    return this->m_qlist_fieldsname.size();
}
