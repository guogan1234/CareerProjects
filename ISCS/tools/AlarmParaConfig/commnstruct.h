#ifndef COMMNSTRUCT_H
#define COMMNSTRUCT_H

#include <QList>
#include <QMap>

struct stru_group_specfunc_area
{
    QList<int> m_vector_perm_group_station;
    QList<int> m_vector_perm_group_region;
};

//用户信息 待定
typedef struct user_info
{
    int m_nuserid = 1;
    int m_nusergid = 1;
    int m_ndomainid = 1;
    int m_nstationid = 1;
    QMap<int,QString> m_map_permission;
    QMap<QString,stru_group_specfunc_area> m_map_specfunc;
    QString m_qstr_hostname;
    bool is_occ;
}UserInfo;

enum ConfigIdStruct
{
    CONFIGID_ALARMMAIN = 1,CONFIGID_ALARMSUMMARY,CONFIGID_EVENTMAIN
};

enum ConfigFields
{
    CONFIGFIELDS_ALARMMAIN = 13,CONFIGFIELDS_ALARMSUMMARY = 13,CONFIGFIELDS_EVENTMAIN = 12
};

enum PermStruct
{
    PERM_STATION = 0,PERM_REGION
};

class AlarmInfoTbl
{
public:
    AlarmInfoTbl();
    ~AlarmInfoTbl();
    char *Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char*>m_qlist_fieldsname;
};

class EventInfoTbl
{
public:
    EventInfoTbl();
    ~EventInfoTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class DomainTbl
{
public:
    DomainTbl();
    ~DomainTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class StationTbl
{
public:
    StationTbl();
    ~StationTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class ProSystemTbl
{
public:
    ProSystemTbl();
    ~ProSystemTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class RegionTbl
{
public:
    RegionTbl();
    ~RegionTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class DevTypeTbl
{
public:
    DevTypeTbl();
    ~DevTypeTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class AlarmLevelTbl
{
public:
    AlarmLevelTbl();
    ~AlarmLevelTbl();
    char * get_field_name(const int index);
    int get_fields_size();
private:
    QList<char *>m_qlist_fieldsname;
};

class AlarmTypeTbl
{
public:
    AlarmTypeTbl();
    ~AlarmTypeTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class AlarmConfigTbl
{
public:
    AlarmConfigTbl();
    ~AlarmConfigTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class PermGroupDefTbl
{
public:
    PermGroupDefTbl();
    ~PermGroupDefTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class PermUserDefTbl
{
public:
    PermUserDefTbl();
    ~PermUserDefTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

class PermUserGroupDefTbl
{
public:
    PermUserGroupDefTbl();
    ~PermUserGroupDefTbl();
    char * Return_FieldsName(const int index);
    int Return_FieldsSize();
private:
    QList<char *>m_qlist_fieldsname;
};

#endif // COMMNSTRUCT_H
