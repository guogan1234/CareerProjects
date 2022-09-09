package com.avp.config.service.system;

import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.system.StationTbl;

import java.util.List;

public interface StationTblService {
    public List<StationTbl> getList(StationTbl stationTbl);

    public StationTbl saveOrUpdate(StationTbl stationTbl,String operate) throws Exception;

    public StationTbl detail(int stationId);

    public void del(int stationId);
}
