package com.avp.cdai.web.model;

import java.util.List;

/**
 * Created by guo on 2017/9/26.
 */
public class MapStationFailed {
    private List<MapStationFailedSub> statusList;

    public List<MapStationFailedSub> getStatusList() {
        return statusList;
    }

    public void setStatusList(List<MapStationFailedSub> statusList) {
        this.statusList = statusList;
    }
}
