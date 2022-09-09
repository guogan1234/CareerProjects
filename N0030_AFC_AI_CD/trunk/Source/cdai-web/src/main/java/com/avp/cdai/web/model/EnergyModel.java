package com.avp.cdai.web.model;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/12.
 */
public class EnergyModel {
    private List<String> modeList;
    private List<Date> dateList;
    private List<Double> energyList;

    public List<String> getModeList() {
        return modeList;
    }

    public void setModeList(List<String> modeList) {
        this.modeList = modeList;
    }

    public List<Date> getDateList() {
        return dateList;
    }

    public void setDateList(List<Date> dateList) {
        this.dateList = dateList;
    }

    public List<Double> getEnergyList() {
        return energyList;
    }

    public void setEnergyList(List<Double> energyList) {
        this.energyList = energyList;
    }
}
