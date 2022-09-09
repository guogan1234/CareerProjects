package com.avp.config.dao.system;

import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.system.StationTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface StationTblRepository extends JpaRepository<StationTbl,Integer>,JpaSpecificationExecutor<StationTbl> {

    public StationTbl save(StationTbl stationTbl);

    public List<StationTbl> findByStationIdOrStationLabelOrStationName(int stationId, String stationLabel, String stationName);

    public List<StationTbl> findByStationLabel(String stationLabel);

    public List<StationTbl> findByStationName(String stationName);

    public StationTbl findByStationId(int stationId);

    public void deleteByStationId(int stationId);

}
