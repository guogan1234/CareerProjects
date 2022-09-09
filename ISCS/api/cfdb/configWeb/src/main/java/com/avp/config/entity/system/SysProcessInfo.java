package com.avp.config.entity.system;

import javax.persistence.*;

/**
 * Created by zhoujs on 2018/5/15.
 */
@Entity
@Table(name = "sys_process_info", schema = "conf")
public class SysProcessInfo {
    private Integer procId;
    private String procAlias;
    private String commandName;
    private Integer instanceNum;
    private Integer notUsed;
    private Integer autoRun;
    private Integer runOrder;
    private Integer cycleStart;
    private Integer cyclePeriod;
    private Integer offlineRun;
    private Integer dutyRun;
    private Integer standbyRun;
    private Integer reportType;
    private Integer occupyContext;
    private String filePath;
    private String commandPara;
    private String description;
    private Integer runStyle;
    private Integer procType;
    private Integer startType;

    @Id
    @Column(name = "proc_id")
    public Integer getProcId() {
        return procId;
    }

    public void setProcId(Integer procId) {
        this.procId = procId;
    }

    @Basic
    @Column(name = "proc_alias")
    public String getProcAlias() {
        return procAlias;
    }

    public void setProcAlias(String procAlias) {
        this.procAlias = procAlias;
    }

    @Basic
    @Column(name = "command_name")
    public String getCommandName() {
        return commandName;
    }

    public void setCommandName(String commandName) {
        this.commandName = commandName;
    }

    @Basic
    @Column(name = "instance_num")
    public Integer getInstanceNum() {
        return instanceNum;
    }

    public void setInstanceNum(Integer instanceNum) {
        this.instanceNum = instanceNum;
    }

    @Basic
    @Column(name = "not_used")
    public Integer getNotUsed() {
        return notUsed;
    }

    public void setNotUsed(Integer notUsed) {
        this.notUsed = notUsed;
    }

    @Basic
    @Column(name = "auto_run")
    public Integer getAutoRun() {
        return autoRun;
    }

    public void setAutoRun(Integer autoRun) {
        this.autoRun = autoRun;
    }

    @Basic
    @Column(name = "run_order")
    public Integer getRunOrder() {
        return runOrder;
    }

    public void setRunOrder(Integer runOrder) {
        this.runOrder = runOrder;
    }

    @Basic
    @Column(name = "cycle_start")
    public Integer getCycleStart() {
        return cycleStart;
    }

    public void setCycleStart(Integer cycleStart) {
        this.cycleStart = cycleStart;
    }

    @Basic
    @Column(name = "cycle_period")
    public Integer getCyclePeriod() {
        return cyclePeriod;
    }

    public void setCyclePeriod(Integer cyclePeriod) {
        this.cyclePeriod = cyclePeriod;
    }

    @Basic
    @Column(name = "offline_run")
    public Integer getOfflineRun() {
        return offlineRun;
    }

    public void setOfflineRun(Integer offlineRun) {
        this.offlineRun = offlineRun;
    }

    @Basic
    @Column(name = "duty_run")
    public Integer getDutyRun() {
        return dutyRun;
    }

    public void setDutyRun(Integer dutyRun) {
        this.dutyRun = dutyRun;
    }

    @Basic
    @Column(name = "standby_run")
    public Integer getStandbyRun() {
        return standbyRun;
    }

    public void setStandbyRun(Integer standbyRun) {
        this.standbyRun = standbyRun;
    }

    @Basic
    @Column(name = "report_type")
    public Integer getReportType() {
        return reportType;
    }

    public void setReportType(Integer reportType) {
        this.reportType = reportType;
    }

    @Basic
    @Column(name = "occupy_context")
    public Integer getOccupyContext() {
        return occupyContext;
    }

    public void setOccupyContext(Integer occupyContext) {
        this.occupyContext = occupyContext;
    }

    @Basic
    @Column(name = "file_path")
    public String getFilePath() {
        return filePath;
    }

    public void setFilePath(String filePath) {
        this.filePath = filePath;
    }

    @Basic
    @Column(name = "command_para")
    public String getCommandPara() {
        return commandPara;
    }

    public void setCommandPara(String commandPara) {
        this.commandPara = commandPara;
    }

    @Basic
    @Column(name = "description")
    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    @Basic
    @Column(name = "run_style")
    public Integer getRunStyle() {
        return runStyle;
    }

    public void setRunStyle(Integer runStyle) {
        this.runStyle = runStyle;
    }

    @Basic
    @Column(name = "proc_type")
    public Integer getProcType() {
        return procType;
    }

    public void setProcType(Integer procType) {
        this.procType = procType;
    }

    @Basic
    @Column(name = "start_type")
    public Integer getStartType() {
        return startType;
    }

    public void setStartType(Integer startType) {
        this.startType = startType;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        SysProcessInfo that = (SysProcessInfo) o;

        if (procId != null ? !procId.equals(that.procId) : that.procId != null) return false;
        if (procAlias != null ? !procAlias.equals(that.procAlias) : that.procAlias != null) return false;
        if (commandName != null ? !commandName.equals(that.commandName) : that.commandName != null) return false;
        if (instanceNum != null ? !instanceNum.equals(that.instanceNum) : that.instanceNum != null) return false;
        if (notUsed != null ? !notUsed.equals(that.notUsed) : that.notUsed != null) return false;
        if (autoRun != null ? !autoRun.equals(that.autoRun) : that.autoRun != null) return false;
        if (runOrder != null ? !runOrder.equals(that.runOrder) : that.runOrder != null) return false;
        if (cycleStart != null ? !cycleStart.equals(that.cycleStart) : that.cycleStart != null) return false;
        if (cyclePeriod != null ? !cyclePeriod.equals(that.cyclePeriod) : that.cyclePeriod != null) return false;
        if (offlineRun != null ? !offlineRun.equals(that.offlineRun) : that.offlineRun != null) return false;
        if (dutyRun != null ? !dutyRun.equals(that.dutyRun) : that.dutyRun != null) return false;
        if (standbyRun != null ? !standbyRun.equals(that.standbyRun) : that.standbyRun != null) return false;
        if (reportType != null ? !reportType.equals(that.reportType) : that.reportType != null) return false;
        if (occupyContext != null ? !occupyContext.equals(that.occupyContext) : that.occupyContext != null)
            return false;
        if (filePath != null ? !filePath.equals(that.filePath) : that.filePath != null) return false;
        if (commandPara != null ? !commandPara.equals(that.commandPara) : that.commandPara != null) return false;
        if (description != null ? !description.equals(that.description) : that.description != null) return false;
        if (runStyle != null ? !runStyle.equals(that.runStyle) : that.runStyle != null) return false;
        if (procType != null ? !procType.equals(that.procType) : that.procType != null) return false;
        if (startType != null ? !startType.equals(that.startType) : that.startType != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = procId != null ? procId.hashCode() : 0;
        result = 31 * result + (procAlias != null ? procAlias.hashCode() : 0);
        result = 31 * result + (commandName != null ? commandName.hashCode() : 0);
        result = 31 * result + (instanceNum != null ? instanceNum.hashCode() : 0);
        result = 31 * result + (notUsed != null ? notUsed.hashCode() : 0);
        result = 31 * result + (autoRun != null ? autoRun.hashCode() : 0);
        result = 31 * result + (runOrder != null ? runOrder.hashCode() : 0);
        result = 31 * result + (cycleStart != null ? cycleStart.hashCode() : 0);
        result = 31 * result + (cyclePeriod != null ? cyclePeriod.hashCode() : 0);
        result = 31 * result + (offlineRun != null ? offlineRun.hashCode() : 0);
        result = 31 * result + (dutyRun != null ? dutyRun.hashCode() : 0);
        result = 31 * result + (standbyRun != null ? standbyRun.hashCode() : 0);
        result = 31 * result + (reportType != null ? reportType.hashCode() : 0);
        result = 31 * result + (occupyContext != null ? occupyContext.hashCode() : 0);
        result = 31 * result + (filePath != null ? filePath.hashCode() : 0);
        result = 31 * result + (commandPara != null ? commandPara.hashCode() : 0);
        result = 31 * result + (description != null ? description.hashCode() : 0);
        result = 31 * result + (runStyle != null ? runStyle.hashCode() : 0);
        result = 31 * result + (procType != null ? procType.hashCode() : 0);
        result = 31 * result + (startType != null ? startType.hashCode() : 0);
        return result;
    }
}
