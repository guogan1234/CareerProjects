package com.avp.config.dao.base;

import com.avp.config.entity.base.RegionTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface RegionTblRepository extends JpaRepository<RegionTbl,Integer>, JpaSpecificationExecutor<RegionTbl> {
    RegionTbl findByRegionId(Integer regionId);

    RegionTbl save(RegionTbl regionTbl);

    List<RegionTbl> findByRegionName(String regionName);

    List<RegionTbl> findByRegionLabel(String regionLabel);

    List<RegionTbl> findByRegionIdOrRegionLabelOrRegionName(int regionId,String regionLabel,String regionName);

    public void deleteByRegionId(Integer regionId);
}
