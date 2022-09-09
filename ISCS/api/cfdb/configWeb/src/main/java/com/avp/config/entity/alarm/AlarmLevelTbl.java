package com.avp.config.entity.alarm;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "alarm_level_tbl",schema = "conf")
public class AlarmLevelTbl {
    @Id
    private  Integer levelId;
    private  String levelName;
    private  Integer priority;
    //private  String color;
    private   String soundFile;
    private   String alarmAct;

    public Integer getLevelId() {
        return levelId;
    }

    public void setLevelId(Integer levelId) {
        this.levelId = levelId;
    }

    public String getLevelName() {
        return levelName;
    }

    public void setLevelName(String levelName) {
        this.levelName = levelName;
    }

    public Integer getPriority() {
        return priority;
    }

    public void setPriority(Integer priority) {
        this.priority = priority;
    }

//    public String getColor() {
//        return color;
//    }
//
//    public void setColor(String color) {
//        this.color = color;
//    }

    public String getSoundFile() {
        return soundFile;
    }

    public void setSoundFile(String soundFile) {
        this.soundFile = soundFile;
    }

    public String getAlarmAct() {
        return alarmAct;
    }

    public void setAlarmAct(String alarmAct) {
        this.alarmAct = alarmAct;
    }
}
