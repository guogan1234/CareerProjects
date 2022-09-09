package com.avp.cdai.web.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.util.Date;

/**
 * Created by guo on 2017/10/10.
 */
@Entity
@Table(schema = "afccd",name = "lighting_system_energy_analysis")
public class LightingSystem {
    @Id
    private Integer id;
    @Column(name = "time_table_mode_id")
    private Integer modeId;
    @Column(name = "time_table_mode_name")
    private String modeName;
    @Column(name = "consume_number")
    private Integer consume;
    @Column(name = "analysis_timestamp")
    private Date timestamp;
    @Column(name = "insert_time")
    private Date insertTime;

    public String getModeName() {
        return modeName;
    }

    public void setModeName(String modeName) {
        this.modeName = modeName;
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getModeId() {
        return modeId;
    }

    public void setModeId(Integer modeId) {
        this.modeId = modeId;
    }

    public Integer getConsume() {
        return consume;
    }

    public void setConsume(Integer consume) {
        this.consume = consume;
    }

    public Date getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Date timestamp) {
        this.timestamp = timestamp;
    }

    public Date getInsertTime() {
        return insertTime;
    }

    public void setInsertTime(Date insertTime) {
        this.insertTime = insertTime;
    }
}
