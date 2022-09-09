package com.avp.config.entity.alarm;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
@Entity
@Table(name = "alarm_act_tbl",schema = "conf")
public class AlarmActTbl {
    @Id
    private  Integer actId;

    private  String actName;

    public Integer getActId() {
        return actId;
    }

    public void setActId(Integer actId) {
        this.actId = actId;
    }

    public String getActName() {
        return actName;
    }

    public void setActName(String actName) {
        this.actName = actName;
    }
}
