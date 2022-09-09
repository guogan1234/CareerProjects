package com.avp.config.entity;

import com.avp.config.entity.alarm.AlarmActTbl;
import com.avp.config.entity.alarm.AlarmLevelTbl;
import com.avp.config.entity.alarm.AlarmTypeTbl;
import com.avp.config.entity.base.*;
import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.entity.system.StationTbl;
import com.avp.config.entity.user.*;

import java.util.ArrayList;
import java.util.List;

public class SelectEntity {
    /**
     * 专业
     */
    List<ProSystemTbl> proSystemTblList=new ArrayList<ProSystemTbl>();
    /**
     * 设备类型
     */
    List<DevTypeTbl> devTypeTblList=new ArrayList<DevTypeTbl>();
    /**
     * 数字量文本标签
     */
    List<DiTextTbl> diTextTblList=new ArrayList<DiTextTbl>();

    /**
     * 公式模版标签
     */
    List<FormulaTpTbl> formulaTpTblList=new ArrayList<FormulaTpTbl>();
    /**
     * 控制动作组标签
     */
    List<DoGrpTbl> doGrpTblList=new ArrayList<DoGrpTbl>();

    List<StationTbl> stationTblList=new ArrayList<StationTbl>();

    List<DomainTbl> domainTblList=new ArrayList<DomainTbl>();

    List<RegionTbl> regionTblList=new ArrayList<RegionTbl>();

    List<UnitTbl> unitTblList=new ArrayList<>();

    List<PermTypeDef> permTypeDefList=new ArrayList<>();


    // 添加AlarmLevel集合 和AlarmType集合
    List<AlarmLevelTbl> alarmLevelTblList = new ArrayList<>();

    List<AlarmTypeTbl> alarmTypeTblList = new ArrayList<>();

    List<AlarmActTbl> alarmActTblList = new ArrayList<>();

    List<PermDepartmentDef> permDepartmentDefList=new ArrayList<>();
    List<PermUserDef> permUserDefList=new ArrayList<>();
    //用户组集合
    List<PermGroupDef> permGroupDefList = new ArrayList<>();
    //权限
    List<PermFuncDef> permFuncDefList=new ArrayList<>();

    public List<ProSystemTbl> getProSystemTblList() {
        return proSystemTblList;
    }

    public void setProSystemTblList(List<ProSystemTbl> proSystemTblList) {
        this.proSystemTblList = proSystemTblList;
    }

    public List<DevTypeTbl> getDevTypeTblList() {
        return devTypeTblList;
    }

    public void setDevTypeTblList(List<DevTypeTbl> devTypeTblList) {
        this.devTypeTblList = devTypeTblList;
    }

    public List<DiTextTbl> getDiTextTblList() {
        return diTextTblList;
    }

    public void setDiTextTblList(List<DiTextTbl> diTextTblList) {
        this.diTextTblList = diTextTblList;
    }


    public List<FormulaTpTbl> getFormulaTpTblList() {
        return formulaTpTblList;
    }

    public void setFormulaTpTblList(List<FormulaTpTbl> formulaTpTblList) {
        this.formulaTpTblList = formulaTpTblList;
    }

    public List<DoGrpTbl> getDoGrpTblList() {
        return doGrpTblList;
    }

    public void setDoGrpTblList(List<DoGrpTbl> doGrpTblList) {
        this.doGrpTblList = doGrpTblList;
    }

    public List<StationTbl> getStationTblList() {
        return stationTblList;
    }

    public void setStationTblList(List<StationTbl> stationTblList) {
        this.stationTblList = stationTblList;
    }

    public List<DomainTbl> getDomainTblList() {
        return domainTblList;
    }

    public void setDomainTblList(List<DomainTbl> domainTblList) {
        this.domainTblList = domainTblList;
    }

    public List<RegionTbl> getRegionTblList() {
        return regionTblList;
    }

    public void setRegionTblList(List<RegionTbl> regionTblList) {
        this.regionTblList = regionTblList;
    }

    public List<UnitTbl> getUnitTblList() {
        return unitTblList;
    }

    public void setUnitTblList(List<UnitTbl> unitTblList) {
        this.unitTblList = unitTblList;
    }

    public List<AlarmLevelTbl> getAlarmLevelTblList() {
        return alarmLevelTblList;
    }

    public void setAlarmLevelTblList(List<AlarmLevelTbl> alarmLevelTblList) {
        this.alarmLevelTblList = alarmLevelTblList;
    }

    public List<AlarmTypeTbl> getAlarmTypeTblList() {
        return alarmTypeTblList;
    }

    public void setAlarmTypeTblList(List<AlarmTypeTbl> alarmTypeTblList) {
        this.alarmTypeTblList = alarmTypeTblList;
    }

    public List<AlarmActTbl> getAlarmActTblList() {
        return alarmActTblList;
    }

    public void setAlarmActTblList(List<AlarmActTbl> alarmActTblList) {
        this.alarmActTblList = alarmActTblList;
    }

    public List<PermTypeDef> getPermTypeDefList() {
        return permTypeDefList;
    }

    public void setPermTypeDefList(List<PermTypeDef> permTypeDefList) {
        this.permTypeDefList = permTypeDefList;
    }

    public List<PermDepartmentDef> getPermDepartmentDefList() {
        return permDepartmentDefList;
    }

    public void setPermDepartmentDefList(List<PermDepartmentDef> permDepartmentDefList) {
        this.permDepartmentDefList = permDepartmentDefList;
    }

    public List<PermUserDef> getPermUserDefList() {
        return permUserDefList;
    }

    public void setPermUserDefList(List<PermUserDef> permUserDefList) {
        this.permUserDefList = permUserDefList;
    }

    public List<PermGroupDef> getPermGroupDefList() {
        return permGroupDefList;
    }

    public void setPermGroupDefList(List<PermGroupDef> permGroupDefList) {
        this.permGroupDefList = permGroupDefList;
    }

    public List<PermFuncDef> getPermFuncDefList() {
        return permFuncDefList;
    }

    public void setPermFuncDefList(List<PermFuncDef> permFuncDefList) {
        this.permFuncDefList = permFuncDefList;
    }
}
