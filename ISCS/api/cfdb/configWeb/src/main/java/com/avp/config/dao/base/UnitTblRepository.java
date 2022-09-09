package com.avp.config.dao.base;

import com.avp.config.entity.base.UnitTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

/**
 * Created by zhoujs on 2018/5/17.
 */
public interface UnitTblRepository extends JpaRepository<UnitTbl,Integer>,JpaSpecificationExecutor<UnitTbl> {

    UnitTbl findByUnitId(Integer unitId);

    UnitTbl save(UnitTbl unitTbl);

    List<UnitTbl> findByUnitIdOrUnitName(int unitId,String unitName);

    List<UnitTbl> findByUnitName(String unitName);

    public void deleteByUnitId(Integer unitId);
}
