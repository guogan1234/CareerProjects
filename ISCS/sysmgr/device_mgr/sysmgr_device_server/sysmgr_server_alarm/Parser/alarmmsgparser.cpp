#include "alarmmsgparser.h"
#include <QJsonObject>
#include <QJsonDocument>

AlarmMsgParser::AlarmMsgParser()
{

}

char *AlarmMsgParser::classToJsonString(AlarmMsg *msg)
{
    QByteArray bArray;
    bArray = classToJsonByteArray(msg);
    if(bArray.isNull() || bArray.isEmpty()){
        qDebug("classToJsonByteArray return NULL!\n");
        return NULL;
    }
    char* temp;
    temp = bArray.data();
    qDebug("alarm msg - %d - %s\n",strlen(temp),temp);
//    delete msg;
//    return temp;
    char* ret = strdup(temp);
    return ret;
}

QByteArray AlarmMsgParser::classToJsonByteArray(AlarmMsg *msg)
{
    QByteArray bArray;
    if(!msg) return bArray;

    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("label",QJsonValue(msg->label));
    obj.insert("time",QJsonValue(msg->time));
    obj.insert("region_id",QJsonValue(msg->region_id));
    obj.insert("station_id",QJsonValue(msg->station_id));
    obj.insert("domain_id",QJsonValue(msg->domain_id));
    obj.insert("dev_type_label",QJsonValue(msg->dev_type_label));
    obj.insert("ctrl_trans_grp_label",QJsonValue(msg->ctrl_trans_grp_label));
    obj.insert("is_restrain",QJsonValue(msg->is_restrain));
    obj.insert("pic_name",QJsonValue(msg->pic_name));
    obj.insert("alarm_mode",QJsonValue(msg->alarm_mode));
    obj.insert("content",QJsonValue(msg->content));
    obj.insert("source_id",QJsonValue(msg->source_id));
    obj.insert("alarm_type",QJsonValue(msg->alarm_type));
    obj.insert("is_del_on_ack",QJsonValue(msg->is_del_on_ack));
    obj.insert("is_replace_alarm",QJsonValue(msg->is_replace_alarm));
    obj.insert("alarm_level",QJsonValue(msg->alarm_level));
    doc.setObject(obj);
    return doc.toJson(QJsonDocument::Compact);
}
