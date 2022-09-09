package com.avp.entity;

/**
 * Created by guo on 2018/4/16.
 */
public class Device {
    String dev_label;
    String dev_name;
    int domain_id;
    int station_id;
    int pro_system_id;
    int region_id;
    int rtu_id;
    String dev_type_label;
    String dev_tp_label;
    String ctrl_trans_grp_label;
    int dev_status;
    int is_summary;
    int summary_value;
    int summary_status;

    public String getDev_label() {
        return dev_label;
    }

    public void setDev_label(String dev_label) {
        this.dev_label = dev_label;
    }

    public String getDev_name() {
        return dev_name;
    }

    public void setDev_name(String dev_name) {
        this.dev_name = dev_name;
    }

    public int getDomain_id() {
        return domain_id;
    }

    public void setDomain_id(int domain_id) {
        this.domain_id = domain_id;
    }

    public int getStation_id() {
        return station_id;
    }

    public void setStation_id(int station_id) {
        this.station_id = station_id;
    }

    public int getPro_system_id() {
        return pro_system_id;
    }

    public void setPro_system_id(int pro_system_id) {
        this.pro_system_id = pro_system_id;
    }

    public int getRegion_id() {
        return region_id;
    }

    public void setRegion_id(int region_id) {
        this.region_id = region_id;
    }

    public int getRtu_id() {
        return rtu_id;
    }

    public void setRtu_id(int rtu_id) {
        this.rtu_id = rtu_id;
    }

    public String getDev_type_label() {
        return dev_type_label;
    }

    public void setDev_type_label(String dev_type_label) {
        this.dev_type_label = dev_type_label;
    }

    public String getDev_tp_label() {
        return dev_tp_label;
    }

    public void setDev_tp_label(String dev_tp_label) {
        this.dev_tp_label = dev_tp_label;
    }

    public String getCtrl_trans_grp_label() {
        return ctrl_trans_grp_label;
    }

    public void setCtrl_trans_grp_label(String ctrl_trans_grp_label) {
        this.ctrl_trans_grp_label = ctrl_trans_grp_label;
    }

    public int getDev_status() {
        return dev_status;
    }

    public void setDev_status(int dev_status) {
        this.dev_status = dev_status;
    }

    public int getIs_summary() {
        return is_summary;
    }

    public void setIs_summary(int is_summary) {
        this.is_summary = is_summary;
    }

    public int getSummary_value() {
        return summary_value;
    }

    public void setSummary_value(int summary_value) {
        this.summary_value = summary_value;
    }

    public int getSummary_status() {
        return summary_status;
    }

    public void setSummary_status(int summary_status) {
        this.summary_status = summary_status;
    }
}
